#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include <algorithm>
#include <iterator>
#include <cstddef>
#include <cassert>
#include <iostream>

/**
	Definizione di una classe templata SparseMatrix che rappresenta una
	matrice sparsa utilizzando una lista per salvare solo i valori della
	matrice che sono diversi dal valore di default.

*/
template <typename T>
class SparseMatrix {
public:
	typedef T value_type;

	/**
		Struttura che rappresenta una casella della matrice.
	*/
	struct element {
		const size_t x; ///< Riga della casella
		const size_t y; ///< Colonna della casella
		value_type value; ///< Valore contenuto


		/**
			Costruttore di un element.
			@param x riga della casella
			@param y colonna della casella
			@param v valore della casella
		*/
		element(size_t x, size_t y, value_type v) : x(x), y(y), value(v) {}

		friend std::ostream& operator<<(std::ostream& os, const element& e) {
			os << "{x : " << e.x
				<< ", y : " << e.y
				<< ", value : " << e.value << "}";

			return os;
		}
	};


private:

	/**
		Struttura che rappresenta un nodo della lista utilizzata per salvare
		i valori della matrice.
	*/
	struct node {
		element *value; ///< puntatore alla struttura che rappresenta una casella
		node *next; ///< puntatore al nodo successivo

		/**
			Costruttore.
			Crea un nodo con valore nullo e senza nodo successivo 
		*/
		node() : next(0), element(0) {}

		/**
			Costruttore di un nodo con un element.
			@param x la riga della casella
			@param y la colonna della casella
			@param val il valore della casella
			@param n il puntatore al nodo successivo. Di default il nodo non punta a nulla
		*/
		node(size_t x, size_t y, value_type val, node *n = 0) : value(new element(x, y, val)), next(n) {}
		

		/**
			Distruttore
		*/
		~node() {
			delete value;
		}
	};


	node *head; ///< puntatore alla testa della lista
	size_t n_rows; ///< numero di righe della matrice
	size_t n_col; ///< numero di colonne della radice
	size_t count; ///< numero di elementi contenuti nella lista
	value_type default_value; ///< il valore delle caselle non contenute nella lista


	/**
		Funzione di appoggio che cancella un nodo della lista e tutti i suoi
		successori ricorsivamente.
	*/
	void _clear(node *n) {
		if(n == 0) {
			return;
		}

		node *tmp = n->next;
		delete n;
		n = 0;
		_clear(tmp);
	}

	
public:

	/**
		Costruttore principale. Costruisce una matrice NxM con tutti i valori
		settati al valore di default.
		@param n numero di righe della matrice
		@param m numero di colonne della matrice
		@param def valore di default
	*/
	SparseMatrix(size_t n, size_t m, value_type def) : count(0),
		default_value(def), n_col(m), n_rows(n), head(0) {
			assert(n > 0);
			assert(m > 0);
		}


	/**
		Copy constructor.
		@param other matrice da copiare
	*/
	SparseMatrix(const SparseMatrix &other) : count(0),
		default_value(static_cast<T>(other.default_value)), n_col(other.n_col), n_rows(other.n_rows), head(0) {
		
		const_iterator i = other.begin();
		const_iterator ie = other.end();

		try {
			while(i != ie) {
				add(i->x, i->y, i->value);
				i++;
			}
		} catch(...) {
			clear();
			throw;
		}
	}


	/**
		Costruttore che costruisce una matrice a partire da una matrice di tipo
		diverso. Il controllo della convertibilita' dei tipo e svolto dal compilatore
		@param other la matrice da copiare convertendo i tipi
	*/
	template <typename Q>
	SparseMatrix(const SparseMatrix<Q> &other) : count(0),
		default_value(other.get_default_value()), n_col(other.get_columns()),
		n_rows(other.get_rows()), head(0){

		typename SparseMatrix<Q>::const_iterator i = other.begin();
		typename SparseMatrix<Q>::const_iterator ie = other.end();

		try {
			while(i != ie) {
				add(i->x, i->y, static_cast<T>(i->value));
				i++;
			}
		} catch(...) {
			clear();
			throw;
		}
	}


	/**
		Distruttore
	*/
	~SparseMatrix() {
		clear();
	}



	/**
		Operatore di assegnamento
		@param other matrice da copiare
		@return la matrice modificata
	*/
	SparseMatrix &operator=(const SparseMatrix &other) {
		if(this != &other) {
			SparseMatrix tmp(other);
			std::swap(head, tmp.head);
			std::swap(n_rows, tmp.n_rows);
			std::swap(n_col, tmp.n_col);
			std::swap(count, tmp.count);
			std::swap(default_value, tmp.default_value);
		}

		return *this;
	}


	/**
		Ritorna il numero di righe della matrice
		@return il numero righe della matrice
	*/
	size_t get_rows() const {
		return n_rows;
	}

	/**
		Ritorna il numero di colonne della matrice
		@return il numero colonne della matrice
	*/
	size_t get_columns() const {
		return n_col;
	}


	/**
		Ritorna il numero di valori che sono stati inseriti nella matrice.
		@return il numero di valori inseriti nella matrice.
	*/
	size_t get_count() const {
		return count;
	}


	/**
		Ritorna il valore di default della matrice
		@return il valore di default della matrice
	*/
	const value_type &get_default_value() const {
		return default_value;
	}


	/**
		Modifica il valore di default della matrice
		@param val il nuovo valore di default
	*/
	void set_default_value(value_type val) {
		default_value = val;
	}


	/**
		Aggiunge un valore alla matrice
		@param x riga della casella da aggiungere
		@param y colonna della casella da aggiungere
		@param value valore della casella da aggiungere
	*/
	void add(size_t x, size_t y, const value_type &value) {
		assert(x < n_rows);
		assert(y < n_col);

		if(head == 0) {
			node* new_node = new node(x, y, value);
			head = new_node;
			count++;
			return;
		}

		node *current = head;
		node *prev = head;

		while(current != 0 && ((current->value->x < x) || (current->value->x <= x && current->value->y <= y))) {
			prev = current;
			current = current->next;
		}

		if(prev->value->x == x && prev->value->y == y) {
			prev->value->value = value;
			return;
		}

		node* new_node = new node(x, y, value);

		if(current == head) {
			new_node->next = head;
			head = new_node;
		} else {
			if(prev->value->x == x && prev->value->y == y) {
				prev->value->value = value;
			} else {
				new_node->next = prev->next;
				prev->next = new_node;
			}
		}

		count++;
	}


	/**
		Ritorna il valore contenuto nella casella [x,y]
		@param x la riga della casella
		@param y la colonna della casella
		@return il valore contenuto nella casella
	*/
	const value_type operator()(size_t x, size_t y) const {
		assert(x < n_rows);
		assert(y < n_col);

		node *current = head;

		while(current != 0) {
			if(current->value->x == x && current->value->y == y)
				return current->value->	value;
			current = current->next;
		}

		return default_value;

	}


	/**
		Funzione che cancella tutti i valori precedentemente inseriti nella
		matrice. Tutti i valori tornano ad essere il valore di default.
	*/
	void clear() {
		_clear(head);
		count = 0;
		head = 0;
	}



// ===================================================================
// ==ITERATOR=========================================================
// ===================================================================

	class const_iterator;

	class iterator {
		node *n;

	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef element                   value_type;
		typedef ptrdiff_t                 difference_type;
		typedef element*     		      pointer;
		typedef element&         	      reference;

	
		iterator() : n(0) {}
		
		iterator(const const_iterator &other) : n(other.n) {}

		const_iterator& operator=(const const_iterator &other) {
			n = other.n;
			return *this;
		}

		~iterator() {}

		// Ritorna il dato riferito dall'iteratore (dereferenziamento)
		reference operator*() {
			return *(n->value);
		}

		// Ritorna il puntatore al dato riferito dall'iteratore
		pointer operator->() {
			return n->value;
		}
		
		// Operatore di iterazione post-incremento
		iterator operator++(int) {
			iterator tmp(*this);
			n = n->next;
			return tmp;
		}

		// Operatore di iterazione pre-incremento
		iterator& operator++() {
			n = n->next;
			return *this;
		}

		// Uguaglianza
		bool operator==(const iterator &other) {
			return (n == other.n);
		}
		
		// Diversita'
		bool operator!=(const iterator &other) {
			return (n != other.n);
		}

	private:
		friend class SparseMatrix;

		iterator(node *nn) : n(nn) {}
		
	}; // classe const_iterator
	
	/**
		Ritorna un iteratore alla prima casella memorizzata
		@return iteratore alla prima casella memorizzata
	*/
	iterator begin() {
		return iterator(head);
	}
	
	/**
		Ritorna un iteratore alla file della sequenza di caselle memorizzate
		@return iteratore alla fine della sequenza di caselle memorizzate
	*/
	iterator end() {
		return iterator(0);
	}
	
// ===================================================================
// ==ITERATOR=========================================================
// ===================================================================
	



// ===================================================================
// ==CONST ITERATOR===================================================
// ===================================================================

		
	class const_iterator {
		node *n;

	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef element                   value_type;
		typedef ptrdiff_t                 difference_type;
		typedef const element*            pointer;
		typedef const element&            reference;

	
		const_iterator() : n(0) {}
		
		const_iterator(const const_iterator &other) : n(other.n) {}

		const_iterator& operator=(const const_iterator &other) {
			n = other.n;
			return *this;
		}

		~const_iterator() {}

		// Ritorna il dato riferito dall'iteratore (dereferenziamento)
		reference operator*() const {
			return *(n->value);
		}

		// Ritorna il puntatore al dato riferito dall'iteratore
		pointer operator->() const {
			return n->value;
		}
		
		// Operatore di iterazione post-incremento
		const_iterator operator++(int) {
			const_iterator tmp(*this);
			n = n->next;
			return tmp;
		}

		// Operatore di iterazione pre-incremento
		const_iterator& operator++() {
			n = n->next;
			return *this;
		}

		// Uguaglianza
		bool operator==(const const_iterator &other) const {
			return (n == other.n);
		}
		
		// Diversita'
		bool operator!=(const const_iterator &other) const {
			return (n != other.n);
		}

	private:
		friend class SparseMatrix;

		const_iterator(node *nn) : n(nn) {}
		
	}; // classe const_iterator
	
	/**
		Ritorna un iteratore alla prima casella memorizzata
		@return iteratore alla prima casella memorizzata
	*/
	const_iterator begin() const {
		return const_iterator(head);
	}
	
	/**
		Ritorna un iteratore alla file della sequenza di caselle memorizzate
		@return iteratore alla fine della sequenza di caselle memorizzate
	*/
	const_iterator end() const {
		return const_iterator(0);
	}
	
// ===================================================================
// ==CONST ITERATOR===================================================
// ===================================================================
	
};


template <typename T>
std::ostream& operator<<(std::ostream &os, const SparseMatrix<T>& sm) {

	typename SparseMatrix<T>::const_iterator i, ie;
	i = sm.begin();
	ie = sm.end();

	while(i != ie) {
		os << *i << std::endl;
		i++;
	}

	return os;
}


/**
	Ritorna il numero di elementi della matrice che soddisfano il predicato
	@param sm matrice sparsa
	@param funct predicato da verificare
	@return il numero di elementi della matrice che soddisfano il predicato
*/
template<typename T, typename P>
int eval(const SparseMatrix<T> &sm, P funct) {
	int result = 0;

	typename SparseMatrix<T>::const_iterator i, ie;
	i = sm.begin();
	ie = sm.end();;

	while(i != ie) {
		if(funct(i->value))
			result++;
		i++;
	}

	if(funct(sm.get_default_value()))
		result += (sm.get_rows() * sm.get_columns()) - sm.get_count();

	return result;
}


#endif
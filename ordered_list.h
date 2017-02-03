#ifndef ORDERED_LIST_H
#define ORDERED_LIST_H

#include <algorithm> // std::swap
#include <iostream>
#include <iterator> // std::forward_iterator_tag
#include <cstddef>  // std::ptrdiff_t

/*
struct funct_generico_3_stati {
	int operator()(const T&a, const T& b) const {
			// return 0 sse a==b
			// return -1 sse a<b
			// return 1 sse a>b
	}
};
*/

/**
	Funtore di comparazione generico per tipi standard che confronta
	i dati in base al loro ordinamento "naturale" indotto dagli operatori
	di confronto standard. 
	Il funtore ritorna -1 se a<b, 0 se a==b e +1 se a>b.

	@brief Funtore di comparazione generico per tipi standard
*/

template <typename T>
struct compare_base {
	int operator()(const T &a, const T&b) const {
		if (a<b) return -1;
		else if (a==b) return 0;
		else return 1;
	}
};

/**
	Classe che implementa una lista ordinata di dati generici T. 
	L'oridnamento e' effettuati utilizzando un funtore di comparazione F.
	Di default il funtore usa l'orinamento naturale dei dati indotto dagli
	operatori di confronto standard. 

	@brief Lista ordinata

	@param T tipo del dato
	@param F funtore di comparazione di due dati
*/
template <typename T, typename F=compare_base<T> >
class ordered_list {

	/**
		Struttura di supporto interna che implementa un nodo della lista.

		@brief Nodo della lista
	*/
	struct node {
		T key; ///< Dato inserito nella lista
		node *next; ///< puntatore al nodo successivo della lista
		
		/**
			Costruttore di default
		*/
		node() : next(0) {}

		/**
			Costruttore secondario che inizializza il nodo
			@param k valore del dato
			@param n puntatore al nodo successivo
		*/
		node(const T &k, node *n=0) : key(k), next(n) {}

		// NOTA: gli altri metodi fondamentali coincidono con quelli
		//       di default
	};

	node *head; ///< puntatore alla testa della lista
	F funct; ///< oggetto funtore per ordinamento
	unsigned int size; ///< numero di elementi nella lista

	/**
		Metodo di supporto per la rimozione dei nodi della lista

		@param n nodo da cancellare

	*/
	void clear_helper(node *n) {
		if(n==0) return;

		node *tmp=n->next;

		delete n; n=0;

		clear_helper(tmp);
	}

public:

	typedef T value_type; ///< Tipo dei dati della lista

	/**
		Costruttore di default
	*/
	ordered_list() : head(0), size(0) {}

	/**
		Distruttore
	*/
	~ordered_list() {
		clear();
	}

	/**
		Costruttore secondario che costruisce la lista a partire
		da una sequenza generica di dati identificata da due 
		iteratori.

		@param begin iteratore di inizio della sequenza
		@param end iteratore di fine della sequenza

		@throw eccezione di allocazione di memoria
	*/
	template <typename IterT>
	ordered_list(IterT begin, IterT end) : head(0), size(0) {
		try {		
			for(; begin!=end; ++begin)
				insert(static_cast<T>(*begin));
		}
		catch(...) {
			clear();
			throw;
		}
	}

	/**
		Costruttore di copia

		@param other lista da copiare
		@throw eccezione di allocazione di memoria
	*/
	ordered_list(const ordered_list &other) : head(0), size(0){
		const node *tmp = other.head;

		try {
			while(tmp!=0)	{
				insert(tmp->key);
				tmp = tmp->next;
			}
		}
		catch(...) {
			clear();
			throw;
		}
	}

	/**
		Operatore di assegnamento
		
		@param other lista da copiare
		@return reference a this

		@throw eccezione di allocazione di memoria
	*/
	ordered_list&operator=(const ordered_list &other){
		if (this != &other) {
			ordered_list tmp(other);
			std::swap(size, tmp.size);
			std::swap(head, tmp.head);
		}

		return *this;
	}

	/**
		Ritorna il numero di elementi nella lista

		@return numero di elementi inseriti
	*/
	unsigned int get_size() const {
		return size;	
	}

	/**
		Cancella il contenuto della lista
	*/
	void clear() {
		clear_helper(head);
		head=0;
		size=0;
	}

	/**
		Inserisce un elemento nella lista secondo l'ordinamento
		definito dal funtore di confronto.

		@param value valore da inserire

		@throw eccezione di allocazione di memoria
	*/
	void insert(const value_type &value) {
		node *current =0;
		
		current = new node(value);

		if(head==0) {
			head = current;
			size++;
			return;
		}
	
		node *n = head, *prev=head;

		while(n!=0 && funct(n->key, value)==-1) {
			prev =n;
			n = n->next;
		}
	
		if(n==head) {
			current->next = head;
			head = current;
			size++;
			return;
		}

		// prev - current - prev->next
		current->next = prev->next;
		prev->next = current;
		size++;
	}

	/**
		Determina se esiste un elemento nella lista. L'uguaglianza
		e' definita dal funtore di confronto.

		@param value valore da cercare

		@return true se esiste l'elemento, false altrimenti
	*/
	bool find(const T &value) const {

		node *n = head;

		while (n!=0 && funct(n->key,value) !=0)
			n = n->next;

		return (n!=0);
	}

	/**
		Stampa il contenuto della lista (per debug)
	*/
	void print() const {
		node *n = head;

		while(n!=0) {
			std::cout<<n->key<<std::endl;
			n = n->next;
		}
	}

	/**
		Iteratore costande della lista

		@brief Iteratore costante della lista
	*/
	class const_iterator {
		node *n;

	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef T                         value_type;
		typedef ptrdiff_t                 difference_type;
		typedef const T*                  pointer;
		typedef const T&                  reference;

	
		const_iterator() : n(0) {	}
		
		const_iterator(const const_iterator &other) : n(other.n){
		}

		const_iterator& operator=(const const_iterator &other) {
			n = other.n;
		}

		~const_iterator() {
		}

		// Ritorna il dato riferito dall'iteratore (dereferenziamento)
		reference operator*() const {
			return n->key;
		}

		// Ritorna il puntatore al dato riferito dall'iteratore
		pointer operator->() const {
			return &(n->key);
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
		// La classe container deve essere messa friend dell'iteratore per poter
		// usare il costruttore di inizializzazione.
		friend class ordered_list; // !!! Da cambiare il nome!

		// Costruttore privato di inizializzazione usato dalla classe container
		// tipicamente nei metodi begin e end
		const_iterator(node *nn) : n(nn) { }

	}; // classe const_iterator
	
	/**
		Ritorna l'iteratore all'inizio della sequenza dati
	
		@return iteratore all'inizio della sequenza
	*/
	const_iterator begin() const {
		return const_iterator(head);
	}
	
	/**
		Ritorna l'iteratore alla fine della sequenza dati
	
		@return iteratore alla fine della sequenza
	*/
	const_iterator end() const {
		return const_iterator(0);
	}
};

/**
	Ridefinizione dell'operatore di stream per la stampa del contenuto
	della lista

	@param os oggetto stream di output
	@param ol lista da stampare

	@return reference allo stream di output
*/
template <typename T, typename F>
std::ostream &operator<<(std::ostream &os, const ordered_list<T, F> &ol) {
	typename ordered_list<T, F>::const_iterator i, ie;

	i = ol.begin();
	ie = ol.end();

	for(; i!=ie; i++)
		os << *i << std::endl;

	return os;
}

#endif

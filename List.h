#ifndef LIST_H
#define LIST_H

#include <iostream>

/********************************** DOUBLE SIDED LIST NODE IMPLEMENTATION **********************************/
template<class D>
class Node {
public:
	Node* prev;
	Node* next;
	D data;

	Node() = default;
	explicit Node(D data) : prev(nullptr), next(nullptr) {
		this->data = data;
	}

	~Node() {
		prev = nullptr;
		next = nullptr;
	}

	Node(const Node<D>& node) { //copy constructor
		prev = node.prev;
		next = node.next;
		data = data(node.data); //using constructor of class D
	}

	Node<D>& operator=(const Node<D>& node) {
		if (this == &node) {
			return *this;
		}
		prev = node.prev;
		next = node.next;
		data = data(node.data);
		return *this;
	}
};

/**********************************************************************************************/

template <class D>
class List {
	Node<D>* head; //dummy nodes
	Node<D>* tail; 
	int size;

public: 
	List();
	List(const List<D>& list); //copy constructor
	~List();
	void insertBeforeNode(D data, Node<D>* node);
	void insestAfterNode(D data, Node<D>* node);
	Node<D>* remove(Node<D>* node);
	Node<D>* find(D data);
	Node<D>* begin();
	Node<D>* end();
	Node<D>* getHead();
	Node<D>* getTail();
	int getSize();
	void destroyNode(Node<D>* to_delete);

};

	/*LIST METHODS IMPLEMENTATIONS */
	template<class D>
	List<D>::List()
	{
		head = new Node<D>();
		tail = new Node<D>();
		head->prev = nullptr;
		head->next = tail;
		tail->prev = head;
		tail->next = nullptr;
		size = 0;
	}

	template<class D>
	List<D>::List(const List<D>& list)
	{
		head = Node<D>(list.head);
		tail = Node<D>(list.tail);
		Node<D>* temp = (list.head)->next;
		Node<D>* current = head;
		while (temp != list.tail) {
			Node<D>* temp1 = Node<D>(temp);
			current->next = temp1;
			temp1->prev = current;
			temp = temp->next; //iteration
			current = current->next; //iteration
		}
		current->next = tail;
		tail->prev = current;
		size = list.size;
	}

	template <class D>
	List<D>::~List() {
		Node<D>* temp = head->next;
		Node<D>* temp_next = temp->next;
		while (temp->next != nullptr) {
			delete temp;
			temp = temp_next; //iteration
			temp_next = temp->next; //iteration
		}
		delete tail;
		delete head;
		size = 0;
	}

	template<class D>
	void List<D>::insertBeforeNode(D data, Node<D>* node)
	{
		Node<D>* new_node = new Node<D>(data);
		(node->prev)->next = new_node;
		new_node->prev = node->prev;
		new_node->next = node;
		node->prev = new_node;
		size++;
	}

	template<class D>
	void List<D>::insestAfterNode(D data, Node<D>* node)
	{
		Node<D>* new_node = new Node<D>(data);
		(node->next)->prev = new_node;
		new_node->next = (node->next);
		node->next = new_node;
		new_node->prev = node;
		size++;
	}

	template<class D>
	Node<D>* List<D>::remove(Node<D>* node)
	{
		Node<D>* tmp = node->prev;
		(node->prev)->next = node->next;
		(node->next)->prev = node->prev;
		size--;
		delete node;
		return tmp;
	}

	template<class D>
	inline Node<D>* List<D>::find(D data)
	{
		Node<D>* node = head;
		while (node->next != nullptr) {
			if (node->data == data) return node;
			node = node->next;
		}
	}

	template<class D>
	Node<D>* List<D>::begin()
	{
		return this->head->next;
	}

	template<class D>
	Node<D>* List<D>::end()
	{
		return this->tail->prev;
	}

	template<class D>
	Node<D>* List<D>::getHead()
	{
		return this->head;
	}

	template<class D>
	Node<D>* List<D>::getTail()
	{
		return this->tail;
	}

	template<class D>
	inline int List<D>::getSize()
	{
		return this->size;
	}

	template<class D>
	void List<D>::destroyNode(Node<D>* to_delete)
	{
		delete to_delete;
	}

#endif // !LIST_H



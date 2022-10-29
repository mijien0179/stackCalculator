#pragma once
#include "ds.hpp"

typedef unsigned int size_t;

template<class T>
class stack {
	dnode<T>* node;
	
public:
	stack() : node{nullptr}
	{

	}
	stack(const stack<T>& src) {
		dnode<T>* tail;

		list_copy(src.node, node, tail);
	}
	~stack() {
		list_clear(node);
		
	}

	void push(T data) {
		list_head_insert(node, data);
	}

	T pop() {
		T out;
		assert(!empty());

		out = node->data();

		list_head_remove(node);

		return out;
	}

	T top() const {
		assert(!empty());

		return node->data();
	}

	bool empty() const{
		if (node) return false;
		return true;
	}

};
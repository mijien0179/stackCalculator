#pragma once
#include <iostream>
#include <iomanip>
#include <cassert>
#include <cstdlib>

template<class value_type>
class dnode {
private:
	value_type data_field;
	dnode* link_fore;
	dnode* link_back;
public:
	/// <summary>
	/// Double Linked List�� �����ϴ� ���
	/// </summary>
	/// <param name="init_data">������ ��</param>
	/// <param name="init_fore">foreward ��� �ּ�</param>
	/// <param name="init_back">backward ��� �ּ�</param>
	dnode(const value_type& init_data = value_type(), const dnode* init_fore = nullptr, const dnode* init_back = nullptr){
		set_data(init_data);
		set_link_foreward(const_cast<dnode*>(init_fore));
		set_link_backward(const_cast<dnode*>(init_back));
		// ������ ���� ���� �� ������ �Է� ����ȭ�� ����
		// Ŭ������ �ʵ忡 ���⸦ ����� ���� �޼��常 ���
	}

	/// <summary>
	/// ������ setter
	/// </summary>
	/// <param name="input">��忡 ������ ������ ��</param>
	void set_data(const value_type& input) {
		data_field = input;
	}
	/// <summary>
	/// forward ��� �ּ� setter
	/// </summary>
	/// <param name="foreward">���� ���� ������ ����� �ּ�</param>
	void set_link_foreward(dnode* foreward) {
		link_fore = foreward;
	}

	/// <summary>
	/// backward ��� �ּ� setter
	/// </summary>
	/// <param name="backward">���� ���� ������ ����� �ּ�</param>
	void set_link_backward(dnode* backward) {
		link_back = backward;
	}
	/// <summary>
	/// ������ getter
	/// </summary>
	/// <returns>���� ��尡 �����ϰ� �ִ� ������</returns>
	value_type data() const {
		return data_field;
	}

	/// <summary>
	/// foreward ��� �ּ� getter
	/// </summary>
	/// <returns>���� ���� ������ �ּ�, �ش� �ּ��� ������ ���� �Ұ�</returns>
	const dnode* link_foreward() const {
		return link_fore;
	}

	/// <summary>
	/// foreward ��� �ּ� getter
	/// </summary>
	/// <returns>���� ���� ������ �ּ�, ��ȯ�� �ּҷ� ������ ���� ����</returns>
	dnode* link_foreward() {
		return link_fore;
	}

	/// <summary>
	/// backward ��� �ּ� getter
	/// </summary>
	/// <returns>���� ���� ������ �ּ�, ��ȯ�� �ּҷ� ������ ���� �Ұ�</returns>
	const dnode* link_backward() const {
		return link_back;
	}

	/// <summary>
	/// backward ��� �ּ� getter
	/// </summary>
	/// <returns>���� ���� ������ �ּ�, ��ȯ�� �ּҷ� ������ ���� ����</returns>
	dnode* link_backward() {
		return link_back;
	}

};

/// <summary>
/// Double Linked List�� ��� ��
/// </summary>
/// <param name="head_ptr">���� ���</param>
/// <returns>����� ��� �� : head�� �����ϴ� ��� 1 ��ȯ</returns>
template<class T>
size_t list_length(const dnode<T>* head_ptr) {
	size_t count = 0;
	const dnode<T>* cursor = head_ptr;

	while (cursor) {
		++count;
		cursor = cursor->link_foreward();
	}
	return count;
}

/// <summary>
/// ���� ����Ʈ ���ۿ� �� ����
/// </summary>
/// <param name="head_ptr">���� ����Ʈ ���� ���</param>
/// <param name="entry">���� ��</param>
template<class T>
void list_head_insert(dnode<T>*& head_ptr, const T& entry) {
	dnode<T>* backup = head_ptr;						// dbl lnk list�� n��° ��忡 n-1��° ��� �ּҸ� backward�� �����ϱ� ���� temp-variable
	head_ptr = new dnode<T>{entry, head_ptr};			// dbl lnk list�� head ��带 ���� ������ ���� ����, �� �������� ���� head ����� �ּҸ� ���� ���� ����� foreward ������ ����
	if (backup) backup->set_link_backward(head_ptr);	// head->sec-node�� backward�� head�� ����
}

/// <summary>
/// ���޵� ����� ������ ���� ����
/// </summary>
/// <param name="previous_ptr">���� ���� ���� ���� ��ġ</param>
/// <param name="entry">���� ��</param>
template<class T>
void list_insert(dnode<T>*& previous_ptr, const T& entry) {
	dnode<T>* backup = previous_ptr;			// ������ �����͸� �ձ� ���� temp-variable
	dnode<T>* insert_ptr;
	if (backup->link_foreward())
		// previous_ptr�� foreward�� ���� ���� ���� �� 
		insert_ptr = new dnode{entry, backup->link_foreward(), backup};
	else
		insert_ptr = new dnode{entry, nullptr, backup};

	backup->set_link_foreward(insert_ptr);
	if (insert_ptr->link_foreward())
		insert_ptr->link_foreward()->set_link_backward(insert_ptr);
	// A - B - C���� A�� previous_ptr, B�� �� �޼��忡�� �����ϴ� ���, C�� previous_ptr->link_foreward���̾���.
	// ���� B�� A, C�� ����Ǿ� �����Ƿ�[LINE 66], A�� foreward�� B��, C�� backward�� B�� �����ϴ� ������ �ʿ��ϴ�.
	// ���� B�� insert_ptr�� forward�� C�� ������ ��, C���� set_link_backward �޼��带 �̿���
	// C.backward�� ���� insert_ptr(B)�� �����ߴ�.
	// ���⿡��, previous_ptr�� ���� �������� ��쿡�� insert_ptr�� previous_ptr ���� ��尡 �Ǳ� ������
	// insert_ptr�� ���� ��尡 �����Ƿ�, nullptr�� �ƴ� ������ C�� ���� �����ϵ��� �б⸦ �ξ���.

}

/// <summary>
/// ����Ʈ���� ���� �˻�
/// </summary>
/// <param name="head_ptr">Ž���� ������ ���</param>
/// <param name="target">��忡�� �˻��� ��</param>
/// <returns>�˻��� ���� ������ ���� ���� ù ���; ��ȯ�� �ּҷ� ��� �� ���� ����</returns>
template<class T>
dnode<T>* list_search(dnode<T>* head_ptr, const T& target) {
	dnode<T>* cursor = head_ptr;
	while (cursor) {
		if (cursor->data() == target) break;
		cursor = cursor->link_foreward();
	}
	return cursor;
}

/// <summary>
/// ����Ʈ���� ���� �˻�
/// </summary>
/// <param name="head_ptr">Ž���� ������ ���</param>
/// <param name="target">��忡�� �˻��� ��</param>
/// <returns>�˻��� ���� ������ ���� ���� ù ���; ��ȯ�� �ּҷ� ��� �� ���� �Ұ�</returns>
template<class T>
const dnode<T>* list_search(const dnode<T>* head_ptr, const T& target) {
	const dnode<T>* cursor = head_ptr;
	while (cursor) {
		if (cursor->data() == target) break;
		cursor = cursor->link_foreward();
	}
	return cursor;
}

/// <summary>
/// ����Ʈ���� ���� �˻�
/// </summary>
/// <param name="head_ptr">Ž���� ������ ���</param>
/// <param name="target">��忡�� �˻��� ��</param>
/// <returns>�˻��� ���� ������ ���� ���� ù ���; ��ȯ�� �ּҷ� ��� �� ���� ����</returns>
template<class T>
const dnode<T>* list_search(std::nullptr_t, const T& target) {
	// nullptr�� �߰��� C++ 11�������ʹ� �� �̻��� ���� �ٸ� ������ Ÿ���� �����ε��ϴ� ���
	// nullptr�� ���� ��ȣ���� ���� �� �����Ƿ� nullptr�� ���� ����� �����ε��� �߰����־���Ѵ�.
	// Reference: https://en.cppreference.com/w/cpp/types/nullptr_t
	return list_search(const_cast<const dnode*>(static_cast<dnode*>(nullptr)), target);
}

/// <summary>
/// ������ ��ġ�� ��� ã��
/// </summary>
/// <param name="head_ptr">������ ���</param>
/// <param name="position">�̵��� ��� ��ġ</param>
/// <returns>��ġ�� �ش��ϴ� ���</returns>
template<class T>
dnode<T>* list_locate(dnode<T>* head_ptr, size_t position) {
	dnode<T>* cursor = head_ptr;
	for (size_t i = 1; i < position && cursor; ++i) {
		cursor = cursor->link_foreward();
	}
	return cursor;
}

/// <summary>
/// ����Ʈ ����
/// </summary>
/// <param name="source_ptr">���� ����Ʈ �ּ�</param>
/// <param name="head_ptr">���� �����ϴ� ����Ʈ ����</param>
/// <param name="tail_ptr">���� �����ϴ� ����Ʈ ��</param>
template<class T>
void list_copy(const dnode<T>* source_ptr, dnode<T>*& head_ptr, dnode<T>*& tail_ptr) {
	head_ptr = tail_ptr = nullptr;

	if (!source_ptr) return;

	list_head_insert(head_ptr, source_ptr->data());
	tail_ptr = head_ptr;

	source_ptr = source_ptr->link_foreward();
	while (source_ptr) {
		list_insert(tail_ptr, source_ptr->data());
		tail_ptr = tail_ptr->link_foreward();
		source_ptr = source_ptr->link_foreward();
	}
}

/// <summary>
/// ����Ʈ ù ��� ����
/// </summary>
/// <param name="head_ptr">������ ����Ʈ ù ���</param>
template<class T>
void list_head_remove(dnode<T>*& head_ptr) {
	// dbl-lnk-list�� backward�� �����ϹǷ� head�� ����Ǿ��� ��
	// backward ������ �Բ� ���־�� �Ѵ�.
	dnode<T>* remove_ptr;
	remove_ptr = head_ptr;
	head_ptr = head_ptr->link_foreward();
	// ������ ����� ��� head_ptr�� nullptr�� �ȴ�.
	if (head_ptr) head_ptr->set_link_backward(nullptr);
	// ���� nullptr�� �ƴ� ��쿡�� ó���ϵ��� �б⸦ �Ѵ�.

	delete remove_ptr;
}

/// <summary>
/// ����Ʈ���� ��� ����
/// </summary>
/// <param name="previous_ptr">������ ����� ���� ���</param>
template<class T>
void list_remove(dnode<T>* previous_ptr) {
	// ���ڷ� �Ѿ�� ����� ���� ��带 ����
	dnode<T>* remove_ptr;
	remove_ptr = previous_ptr->link_foreward();
	previous_ptr->set_link_foreward(remove_ptr->link_foreward());
	// ���� ����� foreward�� ������ ����� �ƴ϶� ���� ��� ���� ���� ����
	remove_ptr->link_foreward()->set_link_backward(previous_ptr);
	// ���� ��� ���� ����� backward ���� ���� ��� ����(previous_ptr)���� ����

	delete remove_ptr;
}

/// <summary>
/// ����Ʈ �Ҵ� ����
/// </summary>
/// <param name="head_ptr">�Ҵ� ������ ����Ʈ</param>
template<class T>
void list_clear(dnode<T>*& head_ptr) {
	while (head_ptr) {
		list_head_remove<T>(head_ptr);
	}
}

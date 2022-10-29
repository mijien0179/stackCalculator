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
	/// Double Linked List를 구성하는 노드
	/// </summary>
	/// <param name="init_data">데이터 값</param>
	/// <param name="init_fore">foreward 노드 주소</param>
	/// <param name="init_back">backward 노드 주소</param>
	dnode(const value_type& init_data = value_type(), const dnode* init_fore = nullptr, const dnode* init_back = nullptr){
		set_data(init_data);
		set_link_foreward(const_cast<dnode*>(init_fore));
		set_link_backward(const_cast<dnode*>(init_back));
		// 데이터 오염 방지 및 데이터 입력 정형화를 위해
		// 클래스의 필드에 쓰기를 진행시 관련 메서드만 사용
	}

	/// <summary>
	/// 데이터 setter
	/// </summary>
	/// <param name="input">노드에 저장할 데이터 값</param>
	void set_data(const value_type& input) {
		data_field = input;
	}
	/// <summary>
	/// forward 노드 주소 setter
	/// </summary>
	/// <param name="foreward">다음 노드로 설정할 노드의 주소</param>
	void set_link_foreward(dnode* foreward) {
		link_fore = foreward;
	}

	/// <summary>
	/// backward 노드 주소 setter
	/// </summary>
	/// <param name="backward">이전 노드로 설정할 노드의 주소</param>
	void set_link_backward(dnode* backward) {
		link_back = backward;
	}
	/// <summary>
	/// 데이터 getter
	/// </summary>
	/// <returns>현재 노드가 저장하고 있는 데이터</returns>
	value_type data() const {
		return data_field;
	}

	/// <summary>
	/// foreward 노드 주소 getter
	/// </summary>
	/// <returns>다음 노드로 설정된 주소, 해당 주소의 데이터 변경 불가</returns>
	const dnode* link_foreward() const {
		return link_fore;
	}

	/// <summary>
	/// foreward 노드 주소 getter
	/// </summary>
	/// <returns>다음 노드로 설정된 주소, 반환된 주소로 데이터 변경 가능</returns>
	dnode* link_foreward() {
		return link_fore;
	}

	/// <summary>
	/// backward 노드 주소 getter
	/// </summary>
	/// <returns>이전 노드로 설정된 주소, 반환된 주소로 데이터 변경 불가</returns>
	const dnode* link_backward() const {
		return link_back;
	}

	/// <summary>
	/// backward 노드 주소 getter
	/// </summary>
	/// <returns>이전 노드로 설정된 주소, 반환된 주소로 데이터 변경 가능</returns>
	dnode* link_backward() {
		return link_back;
	}

};

/// <summary>
/// Double Linked List의 노드 수
/// </summary>
/// <param name="head_ptr">시작 노드</param>
/// <returns>연결된 노드 수 : head만 존재하는 경우 1 반환</returns>
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
/// 연결 리스트 시작에 값 삽입
/// </summary>
/// <param name="head_ptr">연결 리스트 시작 노드</param>
/// <param name="entry">넣을 값</param>
template<class T>
void list_head_insert(dnode<T>*& head_ptr, const T& entry) {
	dnode<T>* backup = head_ptr;						// dbl lnk list에 n번째 노드에 n-1번째 노드 주소를 backward로 저장하기 위한 temp-variable
	head_ptr = new dnode<T>{entry, head_ptr};			// dbl lnk list의 head 노드를 새로 생성한 노드로 수정, 그 과정에서 기존 head 노드의 주소를 새로 만들 노드의 foreward 값으로 설정
	if (backup) backup->set_link_backward(head_ptr);	// head->sec-node의 backward를 head로 설정
}

/// <summary>
/// 전달된 노드의 다음에 값을 삽입
/// </summary>
/// <param name="previous_ptr">값을 넣을 곳의 이전 위치</param>
/// <param name="entry">넣을 값</param>
template<class T>
void list_insert(dnode<T>*& previous_ptr, const T& entry) {
	dnode<T>* backup = previous_ptr;			// 양쪽의 데이터를 잇기 위한 temp-variable
	dnode<T>* insert_ptr;
	if (backup->link_foreward())
		// previous_ptr이 foreward의 값을 갖고 있을 때 
		insert_ptr = new dnode{entry, backup->link_foreward(), backup};
	else
		insert_ptr = new dnode{entry, nullptr, backup};

	backup->set_link_foreward(insert_ptr);
	if (insert_ptr->link_foreward())
		insert_ptr->link_foreward()->set_link_backward(insert_ptr);
	// A - B - C에서 A는 previous_ptr, B는 현 메서드에서 생성하는 노드, C는 previous_ptr->link_foreward값이었다.
	// 현재 B는 A, C와 연결되어 있으므로[LINE 66], A의 foreward를 B로, C의 backward를 B로 수정하는 과정이 필요하다.
	// 따라서 B인 insert_ptr의 forward로 C에 접근한 뒤, C에서 set_link_backward 메서드를 이용해
	// C.backward의 값을 insert_ptr(B)로 수정했다.
	// 여기에서, previous_ptr이 가장 마지막인 경우에는 insert_ptr이 previous_ptr 다음 노드가 되기 때문에
	// insert_ptr의 다음 노드가 없으므로, nullptr이 아닐 때에만 C의 값을 수정하도록 분기를 두었다.

}

/// <summary>
/// 리스트에서 값을 검색
/// </summary>
/// <param name="head_ptr">탐색을 시작할 노드</param>
/// <param name="target">노드에서 검색할 값</param>
/// <returns>검색할 값과 동일한 값을 가진 첫 노드; 반환된 주소로 노드 값 변경 가능</returns>
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
/// 리스트에서 값을 검색
/// </summary>
/// <param name="head_ptr">탐색을 시작할 노드</param>
/// <param name="target">노드에서 검색할 값</param>
/// <returns>검색할 값과 동일한 값을 가진 첫 노드; 반환된 주소로 노드 값 변경 불가</returns>
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
/// 리스트에서 값을 검색
/// </summary>
/// <param name="head_ptr">탐색을 시작할 노드</param>
/// <param name="target">노드에서 검색할 값</param>
/// <returns>검색할 값과 동일한 값을 가진 첫 노드; 반환된 주소로 노드 값 변경 가능</returns>
template<class T>
const dnode<T>* list_search(std::nullptr_t, const T& target) {
	// nullptr이 추가된 C++ 11에서부터는 둘 이상의 서로 다른 포인터 타입을 오버로딩하는 경우
	// nullptr에 의해 모호성을 가질 수 있으므로 nullptr에 대한 명시적 오버로딩을 추가해주어야한다.
	// Reference: https://en.cppreference.com/w/cpp/types/nullptr_t
	return list_search(const_cast<const dnode*>(static_cast<dnode*>(nullptr)), target);
}

/// <summary>
/// 지정된 위치의 노드 찾기
/// </summary>
/// <param name="head_ptr">시작할 노드</param>
/// <param name="position">이동할 노드 위치</param>
/// <returns>위치에 해당하는 노드</returns>
template<class T>
dnode<T>* list_locate(dnode<T>* head_ptr, size_t position) {
	dnode<T>* cursor = head_ptr;
	for (size_t i = 1; i < position && cursor; ++i) {
		cursor = cursor->link_foreward();
	}
	return cursor;
}

/// <summary>
/// 리스트 복제
/// </summary>
/// <param name="source_ptr">원본 리스트 주소</param>
/// <param name="head_ptr">새로 복사하는 리스트 시작</param>
/// <param name="tail_ptr">새로 복사하는 리스트 끝</param>
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
/// 리스트 첫 노드 삭제
/// </summary>
/// <param name="head_ptr">삭제할 리스트 첫 노드</param>
template<class T>
void list_head_remove(dnode<T>*& head_ptr) {
	// dbl-lnk-list는 backward도 포함하므로 head가 변경되었을 때
	// backward 관리를 함께 해주어야 한다.
	dnode<T>* remove_ptr;
	remove_ptr = head_ptr;
	head_ptr = head_ptr->link_foreward();
	// 마지막 노드인 경우 head_ptr은 nullptr이 된다.
	if (head_ptr) head_ptr->set_link_backward(nullptr);
	// 따라서 nullptr이 아닌 경우에만 처리하도록 분기를 한다.

	delete remove_ptr;
}

/// <summary>
/// 리스트에서 노드 삭제
/// </summary>
/// <param name="previous_ptr">삭제할 노드의 이전 노드</param>
template<class T>
void list_remove(dnode<T>* previous_ptr) {
	// 인자로 넘어온 노드의 다음 노드를 삭제
	dnode<T>* remove_ptr;
	remove_ptr = previous_ptr->link_foreward();
	previous_ptr->set_link_foreward(remove_ptr->link_foreward());
	// 이전 노드의 foreward를 삭제할 대상이 아니라 삭제 대상 다음 노드로 수정
	remove_ptr->link_foreward()->set_link_backward(previous_ptr);
	// 삭제 노드 다음 노드의 backward 값을 삭제 노드 이전(previous_ptr)노드로 수정

	delete remove_ptr;
}

/// <summary>
/// 리스트 할당 해제
/// </summary>
/// <param name="head_ptr">할당 해제할 리스트</param>
template<class T>
void list_clear(dnode<T>*& head_ptr) {
	while (head_ptr) {
		list_head_remove<T>(head_ptr);
	}
}

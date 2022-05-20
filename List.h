#pragma once

#include <cstddef> // size_t
#include <iterator> // std::bidirectional_iterator_tag
#include <type_traits> // std::is_same, std::enable_if
#include <iostream>

using namespace std;

template <class T>
class List {
    private:
    struct Node {
        Node *next, *prev;
        T data;
        explicit Node(Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev} {}
        explicit Node(const T& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{data} {}
        explicit Node(T&& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{std::move(data)} {}
    };

    template <typename pointer_type, typename reference_type>
    class basic_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = pointer_type;
        using reference         = reference_type;
    private:
        friend class List<value_type>;
        using Node = typename List<value_type>::Node;

        Node* node;

        explicit basic_iterator(Node* ptr) noexcept : node{ptr} {}
        explicit basic_iterator(const Node* ptr) noexcept : node{const_cast<Node*>(ptr)} {}

    public:
        basic_iterator() : node() {};
        basic_iterator(const basic_iterator&) = default;
        basic_iterator(basic_iterator&&) = default;
        ~basic_iterator() = default;
        basic_iterator& operator=(const basic_iterator&) = default;
        basic_iterator& operator=(basic_iterator&&) = default;

        reference operator*() const {
            return (node->data);
        }
        pointer operator->() const {
            return &(node->data);
        }

        // Prefix Increment: ++a
        basic_iterator& operator++() {
            node = node->next;
            return *this;
        }
        // Postfix Increment: a++
        basic_iterator operator++(int) {
            basic_iterator old = basic_iterator(node);
            node = node->next;
            return old;
        }
        // Prefix Decrement: --a
        basic_iterator& operator--() {
            node = node->prev;
            return *this;
        }
        // Postfix Decrement: a--
        basic_iterator operator--(int) {
            basic_iterator old = basic_iterator(node);
            node = node->prev;
            return old;
        }

        bool operator==(const basic_iterator& other) const noexcept {
            if (node == other.node) {
                return true;
            }
            return false;
        }
        bool operator!=(const basic_iterator& other) const noexcept {
            if (node != other.node) {
                return true;
            }
            return false;
        }
    };

public:
    using value_type      = T;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using iterator        = basic_iterator<pointer, reference>;
    using const_iterator  = basic_iterator<const_pointer, const_reference>;

private:
    Node head, tail;
    size_type _size;

public:
    List() : head{}, tail{} {
        _size = 0;
        head.prev = nullptr;
        tail.next = nullptr;
        head.next = &tail;
        tail.prev = &head;
    }
    List( size_type count, const T& value ) : head{}, tail{} {
        Node* currnode = &head;
        _size = count;
        for (size_t i = 0; i < count; i++) {
            Node* mynode = new Node(value,nullptr,nullptr);
            mynode->prev = currnode;
            currnode->next = mynode;
            currnode = currnode->next;
        }
        currnode->next = &tail;
        tail.prev = currnode;
    }
    explicit List( size_type count ) {
        Node* currnode = &head;
        _size = count;
        for (size_t i = 0; i < count; i++) {
            Node* mynode = new Node({},nullptr,nullptr);
            mynode->prev = currnode;
            currnode->next = mynode;
            currnode = currnode->next;
        }
        currnode->next = &tail;
        tail.prev = currnode;
    }
    List( const List& other ) : _size(other.size()), head{}, tail{} {
        basic_iterator my_iter = other.begin();
        Node* prevnode = &head;
        while (my_iter != other.end()) {
            Node* currnode = new Node(*my_iter,nullptr,nullptr);
            prevnode->next = currnode;
            currnode->prev = prevnode;
            prevnode = currnode;
            my_iter++;
        }
        prevnode->next = &tail;
        tail.prev = prevnode;
    }
    List( List&& other ) : _size(other.size()), head{}, tail{} {
        other.head.next->prev = &(head);
        head.next = other.head.next;
        other.tail.prev->next = &(tail);
        tail.prev = other.tail.prev;

        _size = other.size();
        other._size = 0;
        other.tail.prev = &(other.head);
        other.head.next = &(other.tail);
        other.head.prev = nullptr;
        other.tail.next = nullptr;
    }
    ~List() {
        clear();
    }
    List& operator=( const List& other ) {
        if (other.begin() != begin()) {
            clear();
            basic_iterator my_iter = other.begin();
            Node* prevnode = &head;
            while (my_iter != other.end()) {
                Node* currnode = new Node(*my_iter,nullptr,nullptr);
                prevnode->next = currnode;
                currnode->prev = prevnode;
                prevnode = currnode;
                my_iter++;
            }
            prevnode->next = &tail;
            tail.prev = prevnode;
            _size = other.size();
        }
        return *this;
    }
    List& operator=( List&& other ) noexcept {
        if (other.begin() != begin()) {
            clear();
            other.head.next->prev = &(head);
            head.next = other.head.next;
            other.tail.prev->next = &(tail);
            tail.prev = other.tail.prev;

            _size = other.size();
            other._size = 0;
            other.tail.prev = &(other.head);
            other.head.next = &(other.tail);
            other.head.prev = nullptr;
            other.tail.next = nullptr;
        }
        return *this;
    }

    reference front() {
        return head.next->data;
    }
    const_reference front() const {
        return head.next->data;
    }
	
    reference back() {
        return tail.prev->data;
    }
    const_reference back() const {
        return tail.prev->data;
    }
	
    iterator begin() noexcept {
        iterator beg = iterator(head.next);
        return beg;
    }
    const_iterator begin() const noexcept {
        const_iterator head_iter = const_iterator(head.next);
        return head_iter;
    }
    const_iterator cbegin() const noexcept {
        const_iterator head_iter = const_iterator(head.next);
        return head_iter;
    }

    iterator end() noexcept {
        iterator last = iterator(&tail);
        return last;
    }
    const_iterator end() const noexcept {
        const_iterator tail_iter = const_iterator(&tail);
        return tail_iter;
    }
    const_iterator cend() const noexcept {
        const_iterator tail_iter = const_iterator(&tail);
        return tail_iter;
    }

    bool empty() const noexcept {
        if (_size == 0) {
            return true;
        }
        return false;
    }

    size_type size() const noexcept {
        return _size;
    }

    void clear() noexcept {
        if (empty()) {
            return;
        }
        Node* currnode = head.next;
        Node* nextnode = currnode->next;
        while (currnode != &tail) {
            delete currnode;
            currnode = nextnode;
            nextnode = nextnode->next;
        }
        _size = 0;
    }

    iterator insert( const_iterator pos, const T& value ) {
        Node* mynode = new Node(value, nullptr, nullptr);
        if (empty()) {
            head.next = mynode;
            mynode->prev = &head;
            tail.prev = mynode;
            mynode->next = &tail;
        }
        else {
            Node* currnode = pos.node;
            currnode->prev->next = mynode;
            mynode->prev = currnode->prev;
            mynode->next = currnode;
            currnode->prev = mynode;
        }
        _size++;
        return iterator(mynode);
    }
    iterator insert( const_iterator pos, T&& value ) {
        Node* mynode = new Node(std::move(value), nullptr, nullptr);
        if (empty()) {
            head.next = mynode;
            mynode->prev = &head;
            tail.prev = mynode;
            mynode->next = &tail;
        }
        else {
            Node* currnode = pos.node;
            currnode->prev->next = mynode;
            mynode->prev = currnode->prev;
            mynode->next = currnode;
            currnode->prev = mynode;
        }
        _size++;
        return iterator(mynode);
    }

    iterator erase( const_iterator pos ) {
        Node* currnode = pos.node;
        iterator next = iterator(currnode->next);
        currnode->next->prev = currnode->prev;
        currnode->prev->next = currnode->next;
        
        _size--;
        delete currnode;
        return next;
    }

    void push_back( const T& value ) {
        insert(end(),value);
    }
    void push_back( T&& value ) {
        insert(end(),std::move(value));
    }

    void pop_back() {
        erase(--end());
    }
	
    void push_front( const T& value ) {
        insert(begin(),value);
    }
	void push_front( T&& value ) {
        insert(begin(),std::move(value));
    }

    void pop_front() {
        erase(begin());
    }

    /*
      You do not need to modify these methods!
      
      These method provide the non-const complement 
      for the const_iterator methods provided above.
    */
    iterator insert( iterator pos, const T & value) { 
        return insert((const_iterator &) (pos), value);
    }

    iterator insert( iterator pos, T && value ) {
        return insert((const_iterator &) (pos), std::move(value));
    }

    iterator erase( iterator pos ) {
        return erase((const_iterator&)(pos));
    }
};


/*
    You do not need to modify these methods!

    These method provide a overload to compare const and 
    non-const iterators safely.
*/
 
namespace {
    template<typename Iter, typename ConstIter, typename T>
    using enable_for_list_iters = typename std::enable_if<
        std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::iterator, 
            Iter
        >{} && std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::const_iterator,
            ConstIter
        >{}, T>::type;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) == rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) == lhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) != rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) != lhs;
}
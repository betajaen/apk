// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

namespace apk {

#define MAKE_LIST_NODE(T) \
    T* next;\
    T* prev;\
    typedef T ListNodeType; \
    T* getListNodeDataPtr() {\
        return this;\
    }\
    T& getListNodeDataRef() {\
        return *this;\
    }

    template<typename Node>
    struct ListIterator {
        Node node;

        ListIterator() : node(nullptr) {}
        explicit ListIterator(Node node_) : node(node_) {}

        ListIterator& operator++() {
            if (node) {
                node = node->next;
            }
            return *this;
        }

        ListIterator& operator--() {
            if (node) {
                node = node->prev;
            }
            return *this;
        }

        ListIterator operator++(int) {
            ListIterator<Node> t(node);
            ++(*this);
            return t;
        }

        ListIterator operator--(int) {
            ListIterator<Node> t(node);
            --(*this);
            return t;
        }

        bool isEnd() {
            return node == nullptr;
        }

        Node& operator*() const {
            return node->getListNodeDataRef();
        }

        Node operator->() const {
            return node->getListNodeDataPtr();
        }

		bool operator==(const ListIterator<Node>& other) const {
			return node == other.node;
		}

		bool operator!=(const ListIterator<Node>& other) const {
			return node != other.node;
		}

    };

    template<typename T>
    struct WrappedListNode {
        WrappedListNode<T>* prev;
        WrappedListNode<T>* next;
        T data;
        typedef T ListNodeType;

        WrappedListNode() = default;
        WrappedListNode(const T& v)
            : data(v), prev(nullptr), next(nullptr) {
        }

        ListNodeType* getListNodeDataPtr() {
            return &data;
        }

        ListNodeType& getListNodeDataRef() {
            return data;
        }

    };

    template<typename T>
    class List {
    public:

        typedef T* Node;
        typedef ListIterator<Node> iterator;

        Node head;
        Node tail;

        List() : head(NULL), tail(NULL) {
        }
        ~List() {
            clear();
        }

        iterator begin() {
            return iterator(head);
        }

        iterator end() {
            return iterator(tail);
        }

        bool empty() const {
            return head == nullptr && tail == nullptr;
        }

        Node _createNode(T* value) {
            return value;
        }

        void _destroyNode(Node node) {
        }

        void clear() {
            head = NULL;
            tail = NULL;
        }

        Node findNode(const T& value) {
            Node n = head;
            while(n) {
                if (n->data == value) {
                    return n;
                }
                n = n->next;
            }
            return nullptr;
        }

        void erase(const T& value) {
            Node n = findNode(value);
            assert(n);
            remove(n);
        }

        void remove(Node node) {
            if (head == node && tail == node)
            {
              head = nullptr;
              tail = nullptr;
            }
            else if (head == node)
            {
              head = node->next;
              head->prev = nullptr;
            }
            else if (tail == node)
            {
              tail = node->prev;
              tail->next = nullptr;
            }
            else
            {
              auto after = node->next;
              auto before = node->prev;
              after->prev = before;
              before->next = after;
            }
            node->next = nullptr;
            node->prev = nullptr;
        }

        void push_back(Node node) {
            if (head == nullptr && tail == nullptr) {
                head = tail = node;
            }
            else {
                tail->next = node;
                node->prev = tail;
                tail = node;
            }
        }

        void push_front(Node node) {
            if (head == nullptr && tail == nullptr) {
                head = tail = node;
            }
            else {
                head->prev = node;
                node->next = head;
                head = node;
            }
        }

        void pop_back() {
            if (tail) {
                remove(tail);
            }
        }

        void pop_front() {
            if (head) {
                remove(head);
            }
        }

    };




    template<typename T>
    class WrappedList {
    public:

        typedef WrappedListNode<T>* Node;
        typedef ListIterator<Node> iterator;

        Node head;
        Node tail;

        WrappedList() {
            head = NULL;
            tail = NULL;
        }

        ~WrappedList() {
            clear();
        }

        WrappedList(const WrappedList<T>&) = delete;
        WrappedList(WrappedList<T>&) = delete;

        WrappedList<T>& operator=(const WrappedList<T>& other) = delete;
        WrappedList<T>& operator=(WrappedList<T>&& other) = delete;

        void CopyInto(WrappedList<T>& other) const {
            other.clear();
            Node node = head;
            while(node) {
                other.push_back(node->data);
                node = node->next;
            }
        }

        void MoveInto(WrappedList<T>& other) {
            other.clear();
            other.head = head;
            other.tail = tail;
            head = NULL;
            tail = NULL;
        }

        iterator begin() {
            return iterator(head);
        }

        iterator end() {
            return iterator(tail);
        }

        bool empty() const {
            return head == nullptr && tail == nullptr;
        }

        Node _createNode(const T& value) {
            return apk_new WrappedListNode<T>(value);
        }

        void _destroyNode(Node node) {
            if (node) {
                apk_delete(node);
            }
        }

        Node findNode(const T& value) {
            Node n = head;
            while(n) {
                if (n->data == value) {
                    return n;
                }
                n = n->next;
            }
            return nullptr;
        }

        void erase(const iterator& it) {
            //assert(it.node);
            remove(it.node);
        }

        void clear() {
            Node n = head;
            while(n) {
                Node t = n;
                n = n->next;
                apk_delete(t);
            }
            head = NULL;
            tail = NULL;
        }


        void remove(Node node) {
            if (head == node && tail == node)
            {
              head = nullptr;
              tail = nullptr;
            }
            else if (head == node)
            {
              head = node->next;
              head->prev = nullptr;
            }
            else if (tail == node)
            {
              tail = node->prev;
              tail->next = nullptr;
            }
            else
            {
              auto after = node->next;
              auto before = node->prev;
              after->prev = before;
              before->next = after;
            }
            node->next = nullptr;
            node->prev = nullptr;
            _destroyNode(node);
        }

        void push_back(const T& value) {
            auto node = _createNode(value);
            if (head == nullptr && tail == nullptr) {
                head = tail = node;
            }
            else {
                tail->next = node;
                node->prev = tail;
                tail = node;
            }
        }

        void push_front(const T& value) {
            auto node = _createNode(value);
            if (head == nullptr && tail == nullptr) {
                head = tail = node;
            }
            else {
                head->prev = node;
                node->next = head;
                head = node;
            }
        }

        void pop_back() {
            if (tail) {
                remove(tail);
            }
        }

        void pop_front() {
            if (head) {
                remove(head);
            }
        }

    };


}
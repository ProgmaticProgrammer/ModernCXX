template <typename T>
struct slist_node {
  T data;
  struct slist_node* next;
};

template <typename T>
struct slist {
  slist_node<T>* head;
};

using node_t = struct slist_node<int>;
using list_t = struct slist<int>;

list_t reverse(list_t alist) {
  if (alist.head == nullptr) return;

  list_t reversed_list = {nullptr};
  node_t* next_node = nullptr;

  for (; alist.head; alist.head = next_node) {
    next_node = alist.head->next;
    // pick up a node from head of alist each time
    alist.head->next = reversed_list.head;
    // inset it to the head of reversed_list
    reversed_list.head = alist.head;
  }

  return reversed_list;
}

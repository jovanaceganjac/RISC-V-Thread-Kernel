#ifndef PROJECT_BASE_V1_1_LIST_HPP
#define PROJECT_BASE_V1_1_LIST_HPP

template <typename T>
class List{
private:
    struct Elem{
        T* data;
        Elem *next;
        Elem(T* data, Elem* next) : data(data), next(next) {}
    };
    Elem *head, *tail;

public:
    List() : head(nullptr) , tail(nullptr) {}
    List(const List<T> &)=delete;
    List<T> &operator=(const List<T>&)=delete;

    T* peek(){
        if(!head) return nullptr;
        return head->data;
    }

    void put(T* data){
        Elem *elem=new Elem(data, 0);
        if(tail){
            tail->next=elem;
            tail=elem;
        }
        else{
            head=tail=elem;
        }
    }

    T* get(){
        if(!head) return nullptr;
        Elem *elem=head;
        head=head->next;
        if(!head) tail=nullptr;
        T* ret=elem->data;
        delete elem;
        return ret;
    }
};

#endif

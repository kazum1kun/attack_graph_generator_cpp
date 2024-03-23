#ifndef ATTACKGRAPHGENERATOR_PRIORITYQUEUE_HPP
#define ATTACKGRAPHGENERATOR_PRIORITYQUEUE_HPP

#include "GraphNode.hpp"

class PriorityQueue {
private:
    int capacity;  /* max size of the heap */
    int size;      /* current size of the heap */
    GraphNode **nodes;  /* array of pointers to elements */
public:
    explicit PriorityQueue (int capacity)
    {
       nodes = new GraphNode*[capacity + 1]();
       this->capacity = capacity;
       this->size = 0;
    }
    ~PriorityQueue() {
        delete[] nodes;
    }

    void reset()
    {
        delete[] nodes;
        nodes = new GraphNode*[capacity + 1]();
        size = 0;
    }

    void print()
    {
        int i;
        printf("-----\n");
        printf("capacity=%d, size=%d\n", capacity, size);
        for (i=1; i<=size; i++) printf("%4d::%f, ", nodes[i]->getId(), nodes[i]->getWeight());
        printf("=====\n");
    }

    // Recursion
    void siftUp (int pos)
    {
        int parent;

        parent = pos/2;
        if (pos > 1 && pos <= size && nodes[pos]->getWeight() < nodes[parent]->getWeight())
        {
            std::swap(nodes[pos], nodes[parent]);

            nodes[pos]->setPosInHeap(pos);
            nodes[parent]->setPosInHeap(parent);

            siftUp(parent);
        }
    }

    // Recursion
    void siftDown (int pos)
    {
        int l, r, smallest;

        l = pos*2;
        r = l+1;

        if (l <= size && nodes[l]->getWeight() < nodes[pos]->getWeight())
            smallest = l;
        else
            smallest = pos;

        if (r <= size && nodes[r]->getWeight() < nodes[smallest]->getWeight())
            smallest = r;

        if (smallest != pos){
            std::swap(nodes[pos], nodes[smallest]);

            nodes[pos]->setPosInHeap(pos);
            nodes[smallest]->setPosInHeap(smallest);

            siftDown( smallest);
        }
    }

    void build()
    {
        int i;
        for (i=size/2; i>0; i--) siftDown(i);
    }

    int insert(GraphNode *node)
    {
        if (size >= capacity){
            printf("Error inserting node to pq: pq is full. Capacity=%d, Size=%d\n", capacity, size);
            return 1;
        }
        size += 1;
        nodes[size] = node;
        nodes[size]->setPosInHeap(size);

        siftUp(size);

        return 0;
    }

    int decreaseKey (int pos, double newKey)
    {
        if (pos < 1 || pos > size || newKey >= nodes[pos]->getWeight()){
            printf("Error in decreaseKey\n");
            return 1;
        }

        nodes[pos]->setWeight(newKey);
        siftUp(pos);

        return 0;
    }

    GraphNode *extractMin ()
    {
        GraphNode *min, *last;

        if (size <= 0){
            printf("Error extracting min: pq empty\n");
            return nullptr;
        }
        min = nodes[1];
        min->setPosInHeap(-1);

        last = nodes[size--];
        nodes[1] = last;
        nodes[1]->setPosInHeap(1);

        siftDown(1);

        return min;
    }
};

#endif //ATTACKGRAPHGENERATOR_PRIORITYQUEUE_HPP

#ifndef ATTACKGRAPHGENERATOR_PRIORITYQUEUE_HPP
#define ATTACKGRAPHGENERATOR_PRIORITYQUEUE_HPP

#include "GraphNode.hpp"

class PriorityQueue {
private:
    int capacity;  /* max size of the heap */
    int size;      /* current size of the heap */
    GraphNode *H;  /* array of pointers to elements */
public:
    PriorityQueue (int capacity)
    {

        if (!heap){
            fprintf(stderr, "Error: cannot allocate memory for heap\n");
        }else{
            heap->H = (pVERTEX *) calloc(capacity+1, sizeof(pVERTEX));
            if (!heap->H){
                fprintf(stderr, "Error: cannot allocate memory for heap array\n");
            }
            heap->capacity = capacity;
            heap->size = 0;
        }
        // printf("END heapInit\n");
        return heap;
    }
//===========================================================================


//===========================================================================
// 2022-1026: GLX
//===========================================================================
    void heapReset(HEAP *pHeap)
    {
        // printf("BGN heapReset\n");
        pHeap->size = 0;
        // printf("END heapReset\n");
    }
//===========================================================================


//===========================================================================
// 2022-1026: GLX
//===========================================================================
    void heapFree(HEAP *heap)
    {
        free(heap->H);
        free(heap);
    }
//===========================================================================


//===========================================================================
// 2022-1026: GLX
//===========================================================================
    void heapPrint(HEAP *heap)
    {
        // printf("BGN heapPrint\n");
        int i;
        printf("-----\n");
        printf("capacity=%d, size=%d\n", heap->capacity, heap->size);
        for (i=1; i<heap->size; i++) printf("%4d::%f, ", heap->H[i]->index, heap->H[i]->height);
        if (heap->size > 0) printf("%4d::%f\n", heap->H[i]->index, heap->H[heap->size]->height);
        printf("=====\n");
        // printf("END heapPrint\n");
    }
//===========================================================================


//===========================================================================
// 2022-1026: GLX
//===========================================================================
    void MovingUp (HEAP *heap, int pos)
    {
        pVERTEX temp;
        int parent;

        // printf("BGN MovingUp\n");
        parent = pos/2;
        if (pos > 1 && pos <= heap->size && heap->H[pos]->height < heap->H[parent]->height)
        {
            temp = heap->H[pos];
            heap->H[pos] = heap->H[parent];
            heap->H[parent] = temp;

            heap->H[pos]->pos = pos;
            heap->H[parent]->pos = parent;

            MovingUp(heap, parent);
        }
        // printf("END MovingUp\n");
    }
//===========================================================================


//===========================================================================
// 2022-1026: GLX
//===========================================================================
    void MovingDown (HEAP *heap, int pos)
    {
        pVERTEX temp;
        int l, r, smallest;
        // printf("BGN MovingDown\n");

        l = pos*2; r = l+1;

        if (l <= heap->size && heap->H[l]->height < heap->H[pos]->height)
            smallest = l;
        else
            smallest = pos;

        if (r <= heap->size && heap->H[r]->height < heap->H[smallest]->height)
            smallest = r;

        if (smallest != pos){
            temp = heap->H[smallest];
            heap->H[smallest] = heap->H[pos];
            heap->H[pos] = temp;

            heap->H[smallest]->pos = smallest;
            heap->H[pos]->pos = pos;

            MovingDown(heap, smallest);
            // printf("END MovingDown\n");
        }
    }
//===========================================================================


//===========================================================================
// 2022-1026: GLX
//===========================================================================
    void BuildHeap (HEAP *heap)
    {
        // printf("BGN BuildHeap\n");
        int i;
        for (i=heap->size/2; i>0; i--) MovingDown(heap, i);
        // printf("END BuildHeap\n");
    }
//===========================================================================


//===========================================================================
// 2022-1026: GLX
//===========================================================================
    int HeapInsert(HEAP *heap, pVERTEX item)
    {
        // printf("BGN HeapInsert\n");
        if (heap->size >= heap->capacity){
            printf("Error in HeapInsert: Heap full... Capacity=%d, Size=%d\n", heap->capacity, heap->size);
            // printf("END HeapInsert with Error\n");
            return 1;
        }
        heap->size++;
        heap->H[heap->size] = item;
        heap->H[heap->size]->pos = heap->size;

        MovingUp(heap, heap->size);

        // printf("END HeapInsert\n");
        return 0;
    }
//===========================================================================


//===========================================================================
// 2022-1026: GLX
//===========================================================================
    int DecreaseKey (HEAP *heap, int pos, float newKey)
    {
        // printf("BGN DecreaseKey\n");
        if (pos < 1 || pos > heap->size || newKey >= heap->H[pos]->height){
            printf("Error in DecreaseKey\n");
            // printf("END DecreaseKey with Error\n");
            return 1;
        }

        heap->H[pos]->height = newKey;
        MovingUp(heap, pos);

        // printf("END DecreaseKey\n");
        return 0;
    }
//===========================================================================


//===========================================================================
// 2022-1026: GLX
//===========================================================================
    pVERTEX ExtractMin (HEAP *heap)
    {
        pVERTEX min, last;
        // printf("BGN ExtractMin\n");

        if (heap->size <= 0){
            printf("Error in ExtractMin: heap empty\n");
            // printf("END ExtractMin with Error\n");
            return NULL;
        }
        min = heap->H[1];
        min->pos = -1;

        last = heap->H[heap->size--];
        heap->H[1] = last;
        heap->H[1]->pos = 1;

        MovingDown(heap, 1);

        // printf("END ExtractMin\n");
        return min;
    }
};

#endif //ATTACKGRAPHGENERATOR_PRIORITYQUEUE_HPP

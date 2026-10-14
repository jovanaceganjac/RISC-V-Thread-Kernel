#include "../lib/hw.h"
#include "../h/memory_allocator.hpp"

MemoryAllocator::Blok* MemoryAllocator::head_zauz = nullptr;
MemoryAllocator::Blok* MemoryAllocator::head_slob = nullptr;
bool MemoryAllocator::initialized = false;

const size_t MemoryAllocator::BlokSize = sizeof(MemoryAllocator::Blok);

void MemoryAllocator::init(){
    if(!initialized){
        head_slob=(Blok*)((char*)HEAP_START_ADDR);
        head_slob->size=ROUNDUP((uint64)HEAP_END_ADDR-(uint64)HEAP_START_ADDR+1)/MEM_BLOCK_SIZE-1;
        head_slob->sled = nullptr;
        head_slob->pret  = nullptr;
        initialized=true;
    }
}

void* MemoryAllocator::mem_alloc(size_t size) {
    if (size <= 0) {
        return nullptr;
    }
    //blok je prevelik
    if (size >= ROUNDUP((uint64)HEAP_END_ADDR - (uint64)HEAP_START_ADDR) / MEM_BLOCK_SIZE) {
        return nullptr;
    }
    Blok* trenutni = head_slob;
    //trazi blok dovoljne velicine
    while (trenutni) {
        if (trenutni->size < size) {
            trenutni = trenutni->sled;
            continue;
        }
        //tacno se uklapa
        if (trenutni->size == size) {
            if (trenutni->sled) trenutni->sled->pret = trenutni->pret;
            if (trenutni->pret) trenutni->pret->sled = trenutni->sled;
            if (trenutni == head_slob) head_slob = trenutni->sled;
            trenutni->pret = nullptr;
            trenutni->sled = nullptr;
        }
        //blok je veci
        else {
            trenutni->size -= size;
            trenutni = (Blok*)((char*)trenutni + trenutni->size * MEM_BLOCK_SIZE);
            trenutni->size = size;
            trenutni->pret = nullptr;
            trenutni->sled = nullptr;
        }

        //umetanje u listu zauzetih (sortirano po adr)
        if (!head_zauz) {
            head_zauz = trenutni;
            return (char*)trenutni + BlokSize;
        }

        //ako ide pre head_zauz
        if (trenutni < head_zauz) {
            trenutni->sled = head_zauz;
            head_zauz->pret = trenutni;
            head_zauz = trenutni;
            return (char*)trenutni + BlokSize;
        }

        //pronadji gde da ga ubacis
        Blok* curr = head_zauz;
        while (curr->sled && curr->sled < trenutni) {
            curr = curr->sled;
        }
        trenutni->sled = curr->sled;
        trenutni->pret = curr;

        if (curr->sled) {
            curr->sled->pret = trenutni;
        }
        curr->sled = trenutni;
        return (char*)trenutni + BlokSize;
    }
    return nullptr;
}


int MemoryAllocator::mem_free(void* ptr) {
    if (!ptr) {
        return -1;
    }

    Blok* oslobodjeni = (Blok*)((char*)ptr - BlokSize);

    //nema zauzetih blokova
    if (!head_zauz) {
        return -1;
    }

    //trazi blok u listi zauzetih
    Blok* trenutni = head_zauz;
    while (trenutni && trenutni != oslobodjeni) {
        trenutni = trenutni->sled;
    }

    //blok nije pronadjen
    if (!trenutni) {
        return -1;
    }

    //ukloni iz liste zauzetih
    Blok* prethodni = trenutni->pret;
    Blok* sledeci = trenutni->sled;

    if (prethodni) {
        prethodni->sled = sledeci;
    }
    else {
        head_zauz = sledeci;
    }

    if (sledeci) {
        sledeci->pret = prethodni;
    }

    trenutni->pret = nullptr;
    trenutni->sled = nullptr;

    //nema slobodnih (ide pre head_slob)
    if (!head_slob) {
        head_slob = trenutni;
        return 0;
    }

    //pre prvog slobodnog u listi slobodnih
    if (trenutni < head_slob) {
        trenutni->sled = head_slob;
        head_slob->pret = trenutni;
        head_slob = trenutni;
        tryToJoin(head_slob);
        return 0;
    }

    //umetanje u listu slobodnih (u sredinu, sortirano po adr)
    Blok* curr = head_slob;
    while (curr->sled && curr->sled < trenutni) {
        curr = curr->sled;
    }

    trenutni->sled = curr->sled;
    trenutni->pret = curr;

    if (curr->sled) {
        curr->sled->pret = trenutni;
    }
    curr->sled = trenutni;

    //pokusaj spajanje
    tryToJoin(trenutni);
    tryToJoin(trenutni->pret);

    return 0;
}


void MemoryAllocator::tryToJoin(Blok* tren) {
    if (!tren){
        return;
    }
    Blok* sledeci = tren->sled;
    if (!sledeci){
        return;
    }
    char* krajTrenutnog = (char*)tren + tren->size * MEM_BLOCK_SIZE;
    char* pocetakSledeceg = (char*)sledeci;
    if (krajTrenutnog == pocetakSledeceg) {
        tren->size += sledeci->size;
        tren->sled = sledeci->sled;
        if (tren->sled) tren->sled->pret = tren;
    }
}
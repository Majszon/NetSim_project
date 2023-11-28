//
// Created by HP on 28.11.2023.
//
#include "package.hpp"

Package::Package(){
    ElementID new_id;
    if (!freed_IDs.empty){
        new_id = *freed_IDs.begin();
        freed_IDs.erase(new_id);
    }
    else{
        new_id = assigned_IDs.edn() + 1;
    }
    assigned_IDs.insert(new_id);
    id_ = new_id;
}

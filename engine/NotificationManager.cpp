#include "NotificationManager.h"

namespace CMPUT350 {

    NotificationManager::NotificationManager(<std::string, std::vector<std::weak_ptr<GameObject>>> listeners)
    : mListeners(listeners) {}

    void NotificationManager::Register(std::weak_ptr<GameObject> object, const std::string& key){
        
    }

    void NotificationManager::Unregister(std::weak_ptr<GameObject> object, const std::string& key){

    }
    void NotificationManager::Notify(std::weak_ptr<GameObject> object, const std::string& key){

    }
}


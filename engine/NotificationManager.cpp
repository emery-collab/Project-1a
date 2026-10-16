#include "NotificationManager.h"
#include "GameObject.h"
#include <unordered_set>
namespace CMPUT350 {

    void NotificationManager::Register(std::weak_ptr<GameObject> object, const std::string& key){
        //only register and worry about duplicate logic in other functions. 
        mListeners[key].push_back(object);
        
    }

    void NotificationManager::Unregister(std::weak_ptr<GameObject> object, const std::string& key){
        //remove all objects that are the same within the map. 
        if(auto search = mListeners.find(key); search != mListeners.end()){
            for (size_t x = 0; x < mListeners[key].size(); x++){
                while (x < mListeners[key].size() && mListeners[key].at(x).lock() == object.lock()){
                    mListeners[key].at(x) = mListeners[key].back();
                    mListeners[key].pop_back();
                } 
            }
        }
    }

    void NotificationManager::Notify(const std::string& message){
        if(auto search = mListeners.find(message); search != mListeners.end()){
            //Keep track of objects that have been notified
            std::unordered_set<std::shared_ptr<GameObject>> s;
            size_t x = 0;
            while (x < mListeners[message].size()){
                auto CurrObject = mListeners[message].at(x).lock();
                if(CurrObject != nullptr && CurrObject->IsAlive()){
                    if(s.insert(CurrObject).second){
                        CurrObject->ReceiveNotification(message);
                    }
                }
                else{
                    Unregister(mListeners[message].at(x),message);
                    continue;
                }
                x++;
            }
            
        }

    }
}


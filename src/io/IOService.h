#include <ArduinoJson.h>
#include <vector>

#pragma once

class IOService
{
    public:
    IOService();
    void fetchItems();
    std::vector<String> *getCurrentList();
    std::vector<String> *getAddList();
    std::vector<String> itemsVector;
    std::vector<String> addItemsVector;
    void deleteItem(int itemNo);
    bool addItem(int itemNo);

    private:
    void createWifi();
    void getCredentials();
};
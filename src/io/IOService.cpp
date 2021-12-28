#include "IOService.h"
#include "Arduino.h"
#include "LittleFS.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <vector>
#include "WString.h"

static String nussBotUserName;
static String nussBotPW;

IOService::IOService()
{
    Serial.println("\nStarting IOService...");
    createWifi();
    getCredentials();
    //Serial.printf("%s %s\n", nussBotUserName, nussBotPW);
    fetchItems();
}

void IOService::createWifi()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return;
    }

    if (LittleFS.begin())
    {

        File credentialsFile;
        if (!(credentialsFile = LittleFS.open("/credentials.json", "r")))
        {
            Serial.println("Could not read file credentials.json");
        }

        DynamicJsonDocument doc(1024);
        deserializeJson(doc, credentialsFile.readString());
        Serial.printf("Doc size CREDENTIALS: %d, credentialsFile size:%d\n", doc.size(), credentialsFile.size());
        const char *wifiSSID = doc["wifiSSID"];
        const char *wifiPW = doc["wifiPW"];

        WiFi.begin(wifiSSID, wifiPW);
        //WiFi.begin("Auenland", "MWiD1s.DNssn!");
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(1000);
            Serial.print(".");
        }
        LittleFS.end();
    }
    else
    {
        Serial.println("Could not begin LittleFS");
    }
}

void IOService::fetchItems()
{
    Serial.println("Getting CurrentList...");
    //const char *host = "http://192.168.178.100:8088/shoppinglist/get";
    const char *host = "http://192.168.178.119:8088/shoppinglist/get";

    WiFiClient wiFiClient;
    HTTPClient httpClient;
    httpClient.begin(wiFiClient, host);
    httpClient.addHeader("userid", nussBotUserName);
    httpClient.addHeader("passw", nussBotPW);

    int httpCode = httpClient.GET();
    if (httpCode == HTTP_CODE_OK)
    {
        Serial.print("HTTP response code ");
        Serial.println(httpCode);
        String response = httpClient.getString();
        Serial.println(response);

        // 1024 bytes saved for the array size. Depends on the amount of items ofc
        StaticJsonDocument<600> doc;
        deserializeJson(doc, response);
        Serial.printf("Doc size: %d\n", doc.size());

        itemsVector.clear();
        unsigned int i = 0;
        for (JsonVariant v : doc.as<JsonArray>())
        {
            if (i == doc.size())
            {
                break;
            }

            itemsVector.push_back(v.as<String>());
            i++;
        }
        doc.clear();
    }

    httpClient.end();
    wiFiClient.stop();
}

std::vector<String> *IOService::getCurrentList()
{
    return &itemsVector;
}

std::vector<String> *IOService::getAddList()
{
    Serial.println("Getting AddList...");
    if(addItemsVector.size() > 0) {
        return &addItemsVector;
    }

     if (LittleFS.begin())
    {

        File addListFile;
        if (!(addListFile = LittleFS.open("/items.json", "r")))
        {
            Serial.println("Could not read file items.json");
        }

        DynamicJsonDocument doc(1024);
        deserializeJson(doc, addListFile.readString());
        addItemsVector.clear();
        JsonArray itemsArray = doc["items"].as<JsonArray>();
        Serial.printf("Doc size ITEMS: %d, Array size: %d, itemsFile size:%d\n", doc.size(), itemsArray.size(), addListFile.size());
        unsigned int i = 0;
        for (JsonVariant v : itemsArray)
        {
            if (i == itemsArray.size())
            {
                break;
            }
            Serial.printf("AddListeItem: %s\n", v.as<String>());

            addItemsVector.push_back(v.as<String>());
            i++;
        }
        doc.clear();
        LittleFS.end();
    }
    else
    {
        Serial.println("Could not begin LittleFS for getting AddItems");
    }

    return &addItemsVector;
}

void IOService::deleteItem(int itemNo)
{
    //const char *host = "http://192.168.178.100:8088/shoppinglist/delete";
    const char *host = "http://192.168.178.119:8088/shoppinglist/delete";

    WiFiClient wiFiClient;
    HTTPClient httpClient;
    httpClient.begin(wiFiClient, host);
    httpClient.addHeader("userid", nussBotUserName);
    httpClient.addHeader("passw", nussBotPW);
    httpClient.addHeader("Content-Type", "application/json");

    String item = itemsVector.at(itemNo);
    char itemCharArray[item.length() + 1];
    item.toCharArray(itemCharArray, item.length() + 1);

    char bodyString[100];
    sprintf(bodyString, "{\"item\": \"%s\"}", itemCharArray);
    Serial.printf("body String %s\n", bodyString);

    int httpCode = httpClient.POST(bodyString);
    if (httpCode == HTTP_CODE_OK)
    {
        Serial.print("HTTP response code ");
        Serial.println(httpCode);
        String response = httpClient.getString();
        Serial.println(response);
    }

    httpClient.end();
    wiFiClient.stop();
}

void IOService::getCredentials()
{
    if (LittleFS.begin())
    {

        File credentialsFile;
        if (!(credentialsFile = LittleFS.open("/credentials.json", "r")))
        {
            Serial.println("Could not read file");
        }

        DynamicJsonDocument doc(1024);
        deserializeJson(doc, credentialsFile.readString());
        Serial.printf("Doc size CREDENTIALS: %d, credentialsFile size:%d\n", doc.size(), credentialsFile.size());
        String nussBotUserNameFromDoc = doc["nussbotUser"];
        String nussBotUserPWFromDoc = doc["nussbotPW"];

        nussBotUserName = nussBotUserNameFromDoc;
        nussBotPW = nussBotUserPWFromDoc;
        LittleFS.end();
    }
    else
    {
        Serial.println("Could not begin LittleFS for getting Credentials");
    }
}

bool IOService::addItem(int itemNo) {
    //const char *host = "http://192.168.178.100:8088/shoppinglist/send";
    const char *host = "http://192.168.178.119:8088/shoppinglist/send";

    bool success = false;
    WiFiClient wiFiClient;
    HTTPClient httpClient;
    httpClient.begin(wiFiClient, host);
    httpClient.addHeader("userid", nussBotUserName);
    httpClient.addHeader("passw", nussBotPW);
    httpClient.addHeader("Content-Type", "application/json");

    String item = addItemsVector.at(itemNo);
    char itemCharArray[item.length() + 1];
    item.toCharArray(itemCharArray, item.length() + 1);

    char bodyString[100];
    sprintf(bodyString, "{\"item\": \"%s\"}", itemCharArray);
    Serial.printf("body String %s\n", bodyString);

    int httpCode = httpClient.POST(bodyString);
    if (httpCode == HTTP_CODE_OK)
    {
        success = true;
        Serial.print("HTTP response code ");
        Serial.println(httpCode);
        String response = httpClient.getString();
        Serial.println(response);
    }

    httpClient.end();
    wiFiClient.stop();
    return success;
}
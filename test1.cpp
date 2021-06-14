#include<iostream>
#include<string>
#include<map>
#include<list>
#include <time.h>

std::map<std::string, std::list<float>> x;
void removeRedundantSpaces(std::string *str){
                    for(auto it=str->begin(); it!=str->end();++it){
                        if((*it)==' ' || (*it)=='\n'){
                            it=str->erase(it);
                        }
                    }
                };
std::map<std::string, std::list<float>> createMap(std::string value){
    removeRedundantSpaces(&value);
    bool done=false;
    std::string name;
    std::string intVal;
    std::list<float> p;
    std::map<std::string, std::list<float>> newMap;
    for(auto it=value.begin();it!=value.end();it++){
        while(*it!='('){
            name+=*it;
            ++it;
            if(it==value.end()){
                printf("ERROR");
            }
        }
        ++it;
        while(*it!=')'){
            done=false;
            while(*it!=','){
                intVal+=*it;
                ++it;
                if(*it==')'){
                    done=true;
                    break;
                }
            }
            if(done){
                p.push_back(std::stof(intVal));
                break;
            }
            ++it;
            if(it==value.end()){
                printf("ERROR");
            }
            p.push_back(std::stof(intVal));
            intVal="";
        }
        
        newMap.insert({name, p});
        name="";
        intVal="";
        p.clear();
        
    }
    return newMap;
}
float linearInterpolate(float start, float end, int startFrame, int endFrame, int currentFrame){
    return (float)(end-start)*((currentFrame-startFrame)*1.0/(endFrame-startFrame))+start;
}
/*Possible issues: Length of integer arguments needs to match in current implementation*/
std::map<std::string, std::list<float>> interpolateMap(std::map<std::string, std::list<float>> startMap, std::map<std::string, std::list<float>> endMap, int startKeyframe, int endKeyframe, int currentKeyframe){
    std::map<std::string, std::list<float>> newMap;
    for(auto it=startMap.begin();it!=startMap.end();++it){
        if(endMap.find(it->first)!=endMap.end()){
            auto endValues=endMap.at(it->first);
            std::list<float> newValues;
            auto endValuesIt=endValues.begin();
            for(auto startValuesIt=it->second.begin(); startValuesIt!=it->second.end();++startValuesIt){
                newValues.push_back(linearInterpolate(*startValuesIt, *endValuesIt, startKeyframe, endKeyframe, currentKeyframe));
                ++endValuesIt;
            }
        newMap.insert({it->first, newValues});
        }
        else{
            newMap.insert(*it);
        }
    }
    return newMap;
}
std::string createStringFromMap(std::map<std::string, std::list<float>> dataMap){
    std::string returnString;
    for(auto it=dataMap.begin();it!=dataMap.end();++it){
        returnString.append(it->first);
        std::cout << it->first << "\n";
        returnString.append("(");
        for(auto it2=it->second.begin();it2!=it->second.end();++it2){
            std::cout << "\t" << *it2 << ";\n";
            returnString.append(std::to_string(*it2));
            returnString.append(", ");
        }
        returnString=returnString.substr(0, returnString.size()-2);
        returnString.append(")");
    }
    
    return returnString;
    
}
/*void interpolateMapAndRecreateString(std::map<std::string, std::list<float>> map1, std::map<std::string, std::list<float>> map2){
    std::map<std::string, std::list<float>> newMap;
    for(auto it=map1.begin();it!=map1.end();++it){
        if(map2.find(it->first)){
            int count=0;
            auto t=map2.at(it->first);
            if(t==map2.end()){
                map2.insert({it->first, it->second});
            }
            else{    
                auto itT=t.begin();
                
                for(auto it2=it->second.begin();it2!=it->second.end();++it2){
                    if(itT!=t.end()){
                        printf("Correspoding value found\n");
                    }
                    else{
                        t.push_back(*it2);
                    }
                    it2++;
                    count+=1;
                }
            }
        }
        
    }
};

*/

void displayMap(std::map<std::string, std::list<float>> map1){
    for(auto it=map1.begin();it!=map1.end();++it){
        std::cout << it->first << "\n";
        for(auto it2=it->second.begin();it2!=it->second.end();++it2){
            std::cout << "\t" << *it2 << ";\n";
        }
    }
};
int main() {
    std::map<std::string, std::list<float>> map1, map2;
    //p.insert({"bello",x});
    map1=createMap("rotate(123, 234, 345)transform(100)");
    map2=createMap("rotate(124, 235, 346)transform(200)");
    displayMap(map1);
    displayMap(map2);
    std::cout << createStringFromMap(interpolateMap(map1, map2, 1,3,2));
    
    /*bool found=true;
    std::list<int> z;
    try{
        z=p.at("hello");
    }
    catch(...){
        found=false;
    }
    if(found){
        for(auto pq=z.begin();pq!=z.end();pq++){
            printf("%d\n",*pq);
        }
    }
    else{
        printf("%d ERROR, no such key");
    }*/
}

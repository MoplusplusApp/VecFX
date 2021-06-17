#include <iostream>
#include <string>
#include <cmath>
#include <map>
#include <iterator>
#include <list>
#include <algorithm>
#include <time.h>
#include "pugixml.hpp"
#include "SVGObject.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define CLASS true
#ifdef CLASS

#define ForNodeSet(obj) for (pugi::xpath_node_set::const_iterator it = (obj).begin(); it != (obj).end(); ++it)
using namespace std;

    int randomNumInRange(int rangeStart, int rangeEnd)
    {
        srand((unsigned)time(NULL));
        int randomNumber;
        randomNumber = (rand() % (rangeEnd - rangeStart)) + rangeStart;
        return randomNumber;
    }
    std::string addZeroes(int number, int maxNumber)
    {
        std::string returnVal = "";
        int z = number, count = 0, y = maxNumber, count2 = 0;
        while (z)
        {
            z = z / 10;
            count++;
        }
        while (y)
        {
            y = y / 10;
            count2++;
        }
        if (count2 - count <= 0)
        {
            return std::to_string(number);
        }
        else
        {
            for (int i = 0; i != count2 - count; i++)
            {
                returnVal += "0";
            }
            returnVal += std::to_string(number);
            return returnVal;
        }
    };
    std::map<std::string, int> interpolationMethod = {
        {"stroke-opacity", 1},
        {"fill-opacity", 1},
        {"stroke-width", 1},
        {"stroke-dashoffset", 1},
        {"offset", 1},
        {"stroke-miterlimit", 1},
        {"color", 2},
        {"fill", 2},
        {"transform", 3}};

    float linearInterpolate(float start, float end, int startFrame, int endFrame, int currentFrame)
    {
        return (float)(end - start) * ((currentFrame - startFrame) * 1.0 / (endFrame - startFrame)) + start;
    }

    pugi::xpath_node_set NatronSVGObject::GetObjectsList()
    {
        return this->doc->select_nodes("//*[@id]");
    };
    void NatronSVGObject::SetObjectAttributeAtKeyframe(std::string objectId, std::string attributeName, std::string value, unsigned int keyframeNum, std::string elementType)
        {
            pugi::xml_node keyframeNode = this->keyframeData->child(("k" + std::to_string(keyframeNum)).c_str());
            if (keyframeNode == NULL)
            {
                pugi::xpath_node_set currentKeyframe = this->keyframeData->select_nodes("/*");
                pugi::xpath_node prevKeyframe;
                int thisKeynum, prevKeynum, count = 0;
                pugi::xpath_node presentNodeKeyframe, prevNodeKeyframe;
                bool addedChild = false;
                ForNodeSet(currentKeyframe)
                {
                    presentNodeKeyframe = *it;
                    std::string x = presentNodeKeyframe.node().name();
                    std::cout << endl
                              << "The kf is:" << x.substr(1, x.length() - 1);
                    thisKeynum = std::stoi(x.substr(1, x.length() - 1));
                    if (!count && thisKeynum > keyframeNum)
                    {
                        this->keyframeData->insert_child_before(("k" + std::to_string(keyframeNum)).c_str(), presentNodeKeyframe.node());
                        addedChild = true;
                        break;
                    }
                    else if (count && keyframeNum > prevKeynum && keyframeNum < thisKeynum)
                    {
                        keyframeNode = this->keyframeData->insert_child_before(("k" + std::to_string(keyframeNum)).c_str(), presentNodeKeyframe.node());
                        addedChild = true;
                        break;
                    }
                    prevKeynum = thisKeynum;
                    prevNodeKeyframe = presentNodeKeyframe;
                    count += 1;
                }
                if (!addedChild)
                {
                    keyframeNode = this->keyframeData->append_child(("k" + std::to_string(keyframeNum)).c_str());
                }
            }
            pugi::xml_node objectNode = keyframeNode.child((objectId).c_str());
            if (objectNode == NULL)
            {
                objectNode = keyframeNode.append_child((objectId).c_str());
                objectNode.append_attribute(attributeName.c_str()) = value.c_str();
            }
            else
            {
                if (objectNode.attribute(attributeName.c_str()) == NULL)
                {
                    objectNode.append_attribute(attributeName.c_str()) = value.c_str();
                }
                else
                {
                    objectNode.attribute(attributeName.c_str()) = value.c_str();
                }
            }
            std::cout << "Added attribute at keyframe:" << keyframeNum;
        };
        void NatronSVGObject::writeKeyframeData(std::string filename)
        {
            this->keyframeData->save_file(filename.c_str());
        };
        void NatronSVGObject::calcKeyframePoints()
        {
            for (pugi::xml_node node = this->keyframeData->first_child(); node != NULL; node = node.next_sibling())
            {
                cout << endl
                     << "Point: " << node.name();
                std::string x = node.name();
                this->keyframes.push_back(std::stoi(x.substr(1, x.length() - 1)));
            }
        }
        renderPair NatronSVGObject::findNextReference(pugi::xml_node_iterator currentKeyframeNodeIterator, std::string objectId, std::string propertyName, std::string prevValue)
        {
            renderPair newRenderPair;
            newRenderPair.endValue = "";
            for (pugi::xml_node_iterator keyframeJumper = currentKeyframeNodeIterator; keyframeJumper != this->keyframeData->end(); ++keyframeJumper)
            {
                if (keyframeJumper == currentKeyframeNodeIterator)
                {
                    keyframeJumper++;
                }
                if (!std::string(keyframeJumper->child((objectId).c_str()).attribute(propertyName.c_str()).value()).empty())
                {
                    newRenderPair.endValue = keyframeJumper->child((objectId).c_str()).attribute(propertyName.c_str()).value();
                    std::string x = keyframeJumper->name();
                    //x << "\t" << propertyName << "\t" << keyframeJumper->child((objectId).c_str()).attribute(propertyName.c_str()).value() <<endl;
                    newRenderPair.endKeyframe = std::stoi(x.substr(1, x.length() - 1));
                    if (std::string(keyframeJumper->child((objectId).c_str()).attribute(propertyName.c_str()).value()).empty())
                    {
                        printf("true \n");
                    }
                    else
                    {
                        printf("false \n");
                    }
                    std::cout << "Found next reference \t" << keyframeJumper->child((objectId).c_str()).attribute(propertyName.c_str()).value() << newRenderPair.endKeyframe << endl;
                    break;
                }
            }
            return newRenderPair;
        };
        void NatronSVGObject::updateRenderingData(int keyframe)
        {
            pugi::xml_node_iterator currentKeyframeNodeIterator;
            /*Get the node which matches the current keyframe*/
            if (keyframe == 58)
            {
                std::cout << "here\n";
            }
            for (pugi::xml_node_iterator keyframeIterator = this->keyframeData->begin(); keyframeIterator != keyframeData->end(); ++keyframeIterator)
            {

                if (("k" + std::to_string(keyframe)).compare(keyframeIterator->name()) == 0)
                {
                    currentKeyframeNodeIterator = keyframeIterator;
                    break;
                }
            }

            /*Remove all the properties that end on this keyframe*/
            int count = 0;
            if (!this->renderingData.empty())
            {

                for (auto it = this->renderingData.begin(); it != renderingData.end(); ++it)
                {
                    count += 1; //count++;
                    if (it->endKeyframe == keyframe)
                    {
                        it = this->renderingData.erase(it);
                        while (it->endKeyframe == keyframe)
                        {
                            it = this->renderingData.erase(it);
                        }
                    }
                }
            }

            for (pugi::xml_node_iterator objectIterator = currentKeyframeNodeIterator->begin(); objectIterator != currentKeyframeNodeIterator->end(); ++objectIterator)
            {
                std::cout << "Multiple keyframes found\n";
                std::string x = objectIterator->name();

                for (pugi::xml_attribute_iterator propertyIterator = objectIterator->attributes_begin(); propertyIterator != objectIterator->attributes_end(); ++propertyIterator)
                {
                    renderPair newRenderPair = findNextReference(currentKeyframeNodeIterator, objectIterator->name(), propertyIterator->name(), propertyIterator->value());
                    if (!newRenderPair.endValue.empty())
                    {
                        newRenderPair.startKeyframe = keyframe;
                        newRenderPair.objectId = x;
                        /*TODO: fix property type*/
                        std::cout << "Properties found\n";
                        newRenderPair.propertyType = 1;
                        newRenderPair.propertyName = propertyIterator->name();
                        newRenderPair.startValue = propertyIterator->value();
                        this->renderingData.push_back(newRenderPair);
                    }
                }
            }
        };
        void NatronSVGObject::readKeyframesFromFile(std::string filename)
        {
            pugi::xml_parse_result x = this->keyframeData->load_file(filename.c_str());
            std::cout << x.description() << "First child:" << this->keyframeData->first_child().name() << std::endl;
        };

        //Used for printing PseudoGroups
        void NatronSVGObject::displayPseudoGroups()
        {
            for (map<std::string, std::list<string>>::iterator itr = this->pseudoGroups.begin(); itr != this->pseudoGroups.end(); ++itr)
            {
                cout << itr->first << ":" << endl;
                for (auto it2 = itr->second.begin(); it2 != itr->second.end(); ++it2)
                {
                    cout << "\t" << (*it2) << ";" << endl;
                }
            }
        };
        //Creates a PseudoGroup
        void NatronSVGObject::createPseudoGroup(std::string groupName, std::list<std::string> objectIds)
        {
            this->pseudoGroups.insert({groupName, objectIds});
        };
        //Sets the PseudoGroup's property at a specific keyframe with some more properties.
        void NatronSVGObject::setPseudoGroupAtKeyframe(std::string groupName, std::string attributeName, std::string value, unsigned int keyframeNum, int randRangeStart, int randRangeEnd)
        {
            auto it = this->pseudoGroups.find(groupName);
            srand(time(NULL));
            bool diff = false;
            if (randRangeEnd - randRangeStart > 0)
            {
                for (auto objectIt = it->second.begin(); objectIt != it->second.end(); ++objectIt)
                {
                    int randomisedKf = (rand() % (randRangeEnd - randRangeStart)) + randRangeStart;
                    SetObjectAttributeAtKeyframe((*objectIt), attributeName, value, keyframeNum + randomisedKf);
                }
            }
            else
            {
                for (auto objectIt = it->second.begin(); objectIt != it->second.end(); ++objectIt)
                {
                    SetObjectAttributeAtKeyframe((*objectIt), attributeName, value, keyframeNum);
                }
            }
        };
        //Renders
        void NatronSVGObject::render(std::string folderName)
        {
            int lastKeyframe, counter;
            calcKeyframePoints();
            lastKeyframe = *--(keyframes.end());
            counter = 0; //keyframes.size();
            for (int currentKeyframe = 0; currentKeyframe != lastKeyframe + 1; currentKeyframe++)
            {
                /*Check if the next major keyframe matches the current keyframe*/
                bool majorKeyframe = false;

                if ((std::find(this->keyframes.begin(), this->keyframes.end(), currentKeyframe) != this->keyframes.end()) && currentKeyframe != lastKeyframe)
                {
                    updateRenderingData(currentKeyframe);
                    std::cout << endl
                            << currentKeyframe;
                    displayRenderingData();
                    majorKeyframe = true;
                    counter += 1;
                }
                for (auto updateSVGIter = this->renderingData.begin(); updateSVGIter != this->renderingData.end(); ++updateSVGIter)
                {
                    pugi::xml_node objectNode = (this->doc->select_node(("//*[@id=\"" + updateSVGIter->objectId + "\"]").c_str()).node());

                    if (objectNode != NULL)
                    {
                        std::cout << endl
                                << "Writing keyframe:" << currentKeyframe;
                        if (objectNode.attribute(updateSVGIter->propertyName.c_str()).value() != "")
                        {
                            if (updateSVGIter->propertyName.compare("fill") != 0)
                            {
                                if (updateSVGIter->propertyName.compare("transform") != 0)
                                {
                                    objectNode.attribute(updateSVGIter->propertyName.c_str()).set_value(std::to_string(linearInterpolate((*updateSVGIter), currentKeyframe)).c_str());
                                }
                                else
                                {
                                    objectNode.attribute(updateSVGIter->propertyName.c_str()).set_value(transformInterpolate((*updateSVGIter), currentKeyframe).c_str());
                                }
                            }
                            else
                            {
                                std::cout << endl
                                        << colorInterpolate((*updateSVGIter), currentKeyframe) << " One cycle " << endl;
                                objectNode.attribute(updateSVGIter->propertyName.c_str()).set_value(colorInterpolate((*updateSVGIter), currentKeyframe).c_str());
                            }
                        }
                    }
                }

                this->writeSvg(folderName, (std::to_string(currentKeyframe) + ".svg"), addZeroes(currentKeyframe + 1, lastKeyframe + 1) + ".png");
            }
        };

        void NatronSVGObject::batchRender(string folderName){
            //system(("cd "+ folderName+" && ffmpeg -framerate 60 -i \"%03d.png\" output.mp4 " + std::to_string(lastKeyframe)).c_str());
        }
        void NatronSVGObject::writeSvg(std::string dir, std::string svgFilename, std::string outputFilename)
        {
            this->doc->save_file((dir+"/"+svgFilename).c_str());

        };
        int NatronSVGObject::hex2int(char ch)
        {
            if (ch >= '0' && ch <= '9')
                return ch - '0';
            if (ch >= 'A' && ch <= 'F')
                return ch - 'A' + 10;
            if (ch >= 'a' && ch <= 'f')
                return ch - 'a' + 10;
        };
        std::string NatronSVGObject::int2hex(int x)
        {
            char a[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
            char b[3];
            b[0] = a[(x - (x % 16)) / 16];
            b[1] = a[x % 16];
            b[2] = '\0';
            return b;
        };
        std::string NatronSVGObject::transformInterpolate(renderPair rp, int currentKeyframe){
            std::map<std::string, std::list<float>> map1,map2;
            map1=createMap(rp.startValue);
            map2=createMap(rp.endValue);
            return createStringFromMap(interpolateMap(map1, map2, rp.startKeyframe, rp.endKeyframe, currentKeyframe));
        };
        std::map<std::string, std::list<float>> NatronSVGObject::interpolateMap(std::map<std::string, std::list<float>> startMap, std::map<std::string, std::list<float>> endMap, int startKeyframe, int endKeyframe, int currentKeyframe)
        {
            std::map<std::string, std::list<float>> newMap;
            for (auto it = startMap.begin(); it != startMap.end(); ++it)
            {
                if (endMap.find(it->first) != endMap.end())
                {
                    auto endValues = endMap.at(it->first);
                    std::list<float> newValues;
                    auto endValuesIt = endValues.begin();
                    for (auto startValuesIt = it->second.begin(); startValuesIt != it->second.end(); ++startValuesIt)
                    {
                        newValues.push_back(linearInterpolate(*startValuesIt, *endValuesIt, startKeyframe, endKeyframe, currentKeyframe));
                        ++endValuesIt;
                    }
                    newMap.insert({it->first, newValues});
                }
                else
                {
                    newMap.insert(*it);
                }
            }

            for (auto it = endMap.begin(); it != endMap.end(); ++it)
            {
                if (startMap.find(it->first) == startMap.end())
                {
                    newMap.insert(*it);
                }
            return newMap;
            }
        };
        std::string NatronSVGObject::createStringFromMap(std::map<std::string, std::list<float>> dataMap)
        {
            std::string returnString;
            for (auto it = dataMap.begin(); it != dataMap.end(); ++it)
            {
                returnString.append(it->first);
                returnString.append("(");
                for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                {
                    returnString.append(std::to_string(*it2));
                    returnString.append(", ");
                }
                returnString = returnString.substr(0, returnString.size() - 2);
                returnString.append(")");
            }

            return returnString;
        };
        std::string NatronSVGObject::hexInterpolate(renderPair rp, int currentFrame)
        {
            bool hasA;
            int colorType; //1:HEX(native), 2:RGB
            unsigned char colorValsStart[4], colorValsEnd[4];
            int count = 0;
            std::string ret = "#";
            if (rp.startValue.length() == 9)
            {
                hasA = true;
                auto it2 = rp.endValue.begin();
                for (auto it = rp.startValue.begin(); it != rp.startValue.end(); ++it)
                {
                    if (*it != '#')
                    {
                        colorValsStart[count] = hex2int(*it) * 16;
                        colorValsEnd[count] = hex2int(*it2) * 16;

                        it++;
                        it2++;
                        colorValsStart[count] += hex2int(*it);
                        colorValsEnd[count] += hex2int(*it2);
                        count += 1;
                        ret += int2hex(linearInterpolate(colorValsStart[count], colorValsEnd[count], rp.startKeyframe, rp.endKeyframe, currentFrame));
                    }
                    ++it2;
                }
            }
            else
            {
                hasA = false;
                auto it2 = rp.endValue.begin();
                for (auto it = rp.startValue.begin(); it != rp.startValue.end(); ++it)
                {
                    if (*it != '#')
                    {
                        colorValsStart[count] = hex2int(*it) * 16;
                        colorValsEnd[count] = hex2int(*it2) * 16;
                        ++it;
                        ++it2;
                        colorValsStart[count] += hex2int(*it);
                        colorValsEnd[count] += hex2int(*it2);
                        ret += int2hex(linearInterpolate(colorValsStart[count], colorValsEnd[count], rp.startKeyframe, rp.endKeyframe, currentFrame));
                        count += 1;
                    }
                    ++it2;
                }
            }
            std::cout << endl
                      << ret << endl;
            return ret;
        };
        std::string NatronSVGObject::colorInterpolate(renderPair rp, int currentFrame)
        {
            /*bool hasA;
                    int colorType; //1:HEX(native), 2:RGB
                    unsigned char colorValsStart[4], colorValsEnd[4];
                    int count=0;
                    std::string ret="#";*/
            /*HEX implementation*/
            removeRedundantSpaces(&rp.startValue);
            removeRedundantSpaces(&rp.endValue);
            if (*(rp.startValue.begin()) == '#')
            {
            }
            else if (*(rp.startValue.begin()) == 'r' || *(rp.startValue.begin()) == 'R')
            {
                std::string storage, ret = "#";
                int count = 0;
                unsigned char countVals[4];
                bool passedBracket = false;
                for (auto letterIt = rp.startValue.begin(); letterIt != rp.startValue.end(); ++letterIt)
                {
                    if (!passedBracket)
                    {
                        if (isdigit(*letterIt))
                        {
                            passedBracket = true;
                        }
                    }
                    if (isdigit(*letterIt) && passedBracket)
                    {
                        storage += *letterIt;
                    }
                    else if (passedBracket && !isdigit(*letterIt))
                    {
                        countVals[count] = std::stoi(storage);
                        storage = "";
                        count++;
                    }
                }
                for (int i = 0; i != count; i++)
                {
                    ret += int2hex(countVals[i]);
                }
                rp.startValue = ret;
            }
            if (*(rp.endValue.begin()) == '#')
            {
            }
            else if (*(rp.endValue.begin()) == 'r' || *(rp.endValue.begin()) == 'R')
            {
                std::string storage, ret = "#";
                std::cout << rp.endValue;
                int count = 0;
                unsigned char countVals[4];
                bool passedBracket = false;
                for (auto letterIt = rp.endValue.begin(); letterIt != rp.endValue.end(); ++letterIt)
                {
                    if (!passedBracket)
                    {
                        if (isdigit(*letterIt))
                        {
                            passedBracket = true;
                        }
                    }
                    if (isdigit(*letterIt) && passedBracket)
                    {
                        storage += *letterIt;
                    }
                    else if (passedBracket && !isdigit(*letterIt))
                    {
                        printf("Storage: %s \n", storage.c_str());
                        countVals[count] = std::stoi(storage);
                        storage = "";
                        count++;
                    }
                }
                for (int i = 0; i != count; i++)
                {
                    printf("CountVals[ %d ]: %d ", i, countVals[count]);
                    ret += int2hex(countVals[i]);
                }
                rp.endValue = ret;
            }
            printf("The values are: %s and %s \n", rp.startValue.c_str(), rp.endValue.c_str());
            return hexInterpolate(rp, currentFrame);
            /*if(*(rp.startValue.begin())=='#'){
                        colorType=1;
                        if(rp.startValue.length()==9){
                            hasA=true;
                            auto it2=rp.endValue.begin();
                            for(auto it=rp.startValue.begin();it!=rp.startValue.end();++it){


                                if(*it!='#'){
                                    colorValsStart[count]=hex2int(*it)*16;
                                    colorValsEnd[count]=hex2int(*it2)*16;

                                    it++;
                                    it2++;
                                    colorValsStart[count]+=hex2int(*it);
                                    colorValsEnd[count]+=hex2int(*it2);
                                    std::cout << endl << " Important data: " << hex2int(*it) << "   " << hex2int(*it2) << endl;
                                    count+=1;

                                    //std::cout << endl << " Important data: " << *it << "   " << colorValsEnd[count] << endl;//linearInterpolate(colorValsStart[count], colorValsEnd[count], rp.startKeyframe, rp.endKeyframe, currentFrame);// << "    " << int2hex(linearInterpolate(colorValsStart[count], colorValsEnd[count], rp.startKeyframe, rp.endKeyframe, currentFrame)) << endl;
                                    cout << "Checking:" << colorValsStart[count] << "   " << colorValsEnd[count];
                                    ret+=int2hex(linearInterpolate(colorValsStart[count], colorValsEnd[count], rp.startKeyframe, rp.endKeyframe, currentFrame));
                                }
                                ++it2;
                            }
                        }
                        else{
                            hasA=false;
                            auto it2=rp.endValue.begin();
                            for(auto it=rp.startValue.begin();it!=rp.startValue.end();++it){
                                if(*it!='#'){
                                    colorValsStart[count]=hex2int(*it)*16;
                                    colorValsEnd[count]=hex2int(*it2)*16;
                                    printf("Debug logA: %c \n", (*it2));

                                    //printf("The colorVals part 1 is: %d \n", hex2int(*it2)*16);
                                    ++it;
                                    ++it2;
                                    colorValsStart[count]+=hex2int(*it);
                                    //printf("The colorValsEnd is: %d \n", colorValsEnd[count]);
                                    colorValsEnd[count]+=hex2int(*it2);
                                    printf("Debug logB: %c \n", (*it2));
                                    if(count==1){
                                        printf("Debug log: %d \n", colorValsEnd[count]);
                                    }

                                    ret+=int2hex(linearInterpolate(colorValsStart[count], colorValsEnd[count], rp.startKeyframe, rp.endKeyframe, currentFrame));
                                    count+=1;
                                }
                                ++it2;
                            }
                        }
                        std::cout << endl << ret << endl;
                        return ret;
                    }
                    //RGB comparison
                    else if(*(rp.startValue.begin())=='r')
                    {

                    }*/
        };

        void NatronSVGObject::removeRedundantSpaces(std::string *str)
        {
            for (auto it = str->begin(); it != str->end(); ++it)
            {
                if ((*it) == ' ' || (*it) == '\n')
                {
                    it = str->erase(it);
                }
            }
        };
        float NatronSVGObject::linearInterpolate(renderPair rp, int currentFrame)
        {
            return (float)((std::stof(rp.endValue) - std::stof(rp.startValue)) * ((currentFrame - rp.startKeyframe) * 1.0 / (rp.endKeyframe - rp.startKeyframe)) + std::stof(rp.startValue));
        };
        float NatronSVGObject::linearInterpolate(float startValue, float endValue, int startKeyframe, int endKeyframe, int currentFrame)
        {
            return (float)((endValue - startValue) * ((currentFrame - startKeyframe) * 1.0 / (endKeyframe - startKeyframe)) + startValue);
        };
        pugi::xml_document keyframeData;
        std::list<renderPair> renderingData;
        void NatronSVGObject::displayRenderPair(renderPair p)
        {
            std::cout << "Object ID:" << p.objectId << "Property Name:" << p.propertyName << "Start value:" << p.startValue << "End:" << p.endValue << "Start keyframe:" << p.startKeyframe << "End keyframe:" << p.endKeyframe;
        };
        std::map<std::string, std::list<float>> NatronSVGObject::createMap(std::string value){
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
        };

        void NatronSVGObject::displayRenderingData()
        {
            std::cout << "Hey there";
            for (auto it = this->renderingData.begin(); it != this->renderingData.end(); ++it)
            {
                std::cout << endl
                          << "Object ID: " << it->objectId << " Property Name: " << it->propertyName << " Start value: " << it->startValue << " End:" << it->endValue << " Start keyframe:" << it->startKeyframe << " End keyframe:" << it->endKeyframe;
            }
        };


    #ifdef LOCAL
    class NatronSVGObject
    {
    public:

        /*pugi::xml_document *doc;

        std::list<std::list<std::string>> propertyMap;
        std::list<int> keyframes;*/

        /*XML STRUTCTURE FOR KEYFRAME DATA
            <keyframeNum>
                <objectId1 attribute1=value1 attribute2=value2 attribute3=value3 ...>
                <objectId2 attribute1=value1 attribute2=value2 attribute3=value3 ...>
            </keyframeNum>
            */
        NatronSVGObject(){};
        pugi::xpath_node_set GetObjectsList()
        {
            return this->doc->select_nodes("//*[@id]");
        };
        void SetObjectAttributeAtKeyframe(std::string objectId, std::string attributeName, std::string value, unsigned int keyframeNum, std::string elementType = "*")
        {
            pugi::xml_node keyframeNode = this->keyframeData->child(("k" + std::to_string(keyframeNum)).c_str());
            if (keyframeNode == NULL)
            {
                pugi::xpath_node_set currentKeyframe = this->keyframeData->select_nodes("/*");
                pugi::xpath_node prevKeyframe;
                int thisKeynum, prevKeynum, count = 0;
                pugi::xpath_node presentNodeKeyframe, prevNodeKeyframe;
                bool addedChild = false;
                ForNodeSet(currentKeyframe)
                {
                    presentNodeKeyframe = *it;
                    std::string x = presentNodeKeyframe.node().name();
                    std::cout << endl
                              << "The kf is:" << x.substr(1, x.length() - 1);
                    thisKeynum = std::stoi(x.substr(1, x.length() - 1));
                    if (!count && thisKeynum > keyframeNum)
                    {
                        this->keyframeData->insert_child_before(("k" + std::to_string(keyframeNum)).c_str(), presentNodeKeyframe.node());
                        addedChild = true;
                        break;
                    }
                    else if (count && keyframeNum > prevKeynum && keyframeNum < thisKeynum)
                    {
                        keyframeNode = this->keyframeData->insert_child_before(("k" + std::to_string(keyframeNum)).c_str(), presentNodeKeyframe.node());
                        addedChild = true;
                        break;
                    }
                    prevKeynum = thisKeynum;
                    prevNodeKeyframe = presentNodeKeyframe;
                    count += 1;
                }
                if (!addedChild)
                {
                    keyframeNode = this->keyframeData->append_child(("k" + std::to_string(keyframeNum)).c_str());
                }
            }
            pugi::xml_node objectNode = keyframeNode.child((objectId).c_str());
            if (objectNode == NULL)
            {
                objectNode = keyframeNode.append_child((objectId).c_str());
                objectNode.append_attribute(attributeName.c_str()) = value.c_str();
            }
            else
            {
                if (objectNode.attribute(attributeName.c_str()) == NULL)
                {
                    objectNode.append_attribute(attributeName.c_str()) = value.c_str();
                }
                else
                {
                    objectNode.attribute(attributeName.c_str()) = value.c_str();
                }
            }
            std::cout << "Added attribute at keyframe:" << keyframeNum;
        };
        void writeKeyframeData(std::string filename)
        {
            this->keyframeData->save_file(filename.c_str());
        };
        void calcKeyframePoints()
        {
            for (pugi::xml_node node = this->keyframeData->first_child(); node != NULL; node = node.next_sibling())
            {
                cout << endl
                     << "Point: " << node.name();
                std::string x = node.name();
                this->keyframes.push_back(std::stoi(x.substr(1, x.length() - 1)));
            }
        }
        renderPair findNextReference(pugi::xml_node_iterator currentKeyframeNodeIterator, std::string objectId, std::string propertyName, std::string prevValue)
        {
            renderPair newRenderPair;
            newRenderPair.endValue = "";
            for (pugi::xml_node_iterator keyframeJumper = currentKeyframeNodeIterator; keyframeJumper != this->keyframeData->end(); ++keyframeJumper)
            {
                if (keyframeJumper == currentKeyframeNodeIterator)
                {
                    keyframeJumper++;
                }
                if (!std::string(keyframeJumper->child((objectId).c_str()).attribute(propertyName.c_str()).value()).empty())
                {
                    newRenderPair.endValue = keyframeJumper->child((objectId).c_str()).attribute(propertyName.c_str()).value();
                    std::string x = keyframeJumper->name();
                    //x << "\t" << propertyName << "\t" << keyframeJumper->child((objectId).c_str()).attribute(propertyName.c_str()).value() <<endl;
                    newRenderPair.endKeyframe = std::stoi(x.substr(1, x.length() - 1));
                    if (std::string(keyframeJumper->child((objectId).c_str()).attribute(propertyName.c_str()).value()).empty())
                    {
                        printf("true \n");
                    }
                    else
                    {
                        printf("false \n");
                    }
                    std::cout << "Found next reference \t" << keyframeJumper->child((objectId).c_str()).attribute(propertyName.c_str()).value() << newRenderPair.endKeyframe << endl;
                    break;
                }
            }
            return newRenderPair;
        }
        void updateRenderingData(int keyframe)
        {
            pugi::xml_node_iterator currentKeyframeNodeIterator;
            /*Get the node which matches the current keyframe*/
            if (keyframe == 58)
            {
                std::cout << "here\n";
            }
            for (pugi::xml_node_iterator keyframeIterator = this->keyframeData->begin(); keyframeIterator != keyframeData.end(); ++keyframeIterator)
            {

                if (("k" + std::to_string(keyframe)).compare(keyframeIterator->name()) == 0)
                {
                    currentKeyframeNodeIterator = keyframeIterator;
                    break;
                }
            }

            /*Remove all the properties that end on this keyframe*/
            int count = 0;
            if (!this->renderingData.empty())
            {

                for (auto it = this->renderingData.begin(); it != renderingData.end(); ++it)
                {
                    count += 1; //count++;
                    if (it->endKeyframe == keyframe)
                    {
                        it = this->renderingData.erase(it);
                        while (it->endKeyframe == keyframe)
                        {
                            it = this->renderingData.erase(it);
                        }
                    }
                }
            }

            for (pugi::xml_node_iterator objectIterator = currentKeyframeNodeIterator->begin(); objectIterator != currentKeyframeNodeIterator->end(); ++objectIterator)
            {
                std::cout << "Multiple keyframes found\n";
                std::string x = objectIterator->name();

                for (pugi::xml_attribute_iterator propertyIterator = objectIterator->attributes_begin(); propertyIterator != objectIterator->attributes_end(); ++propertyIterator)
                {
                    renderPair newRenderPair = findNextReference(currentKeyframeNodeIterator, objectIterator->name(), propertyIterator->name(), propertyIterator->value());
                    if (!newRenderPair.endValue.empty())
                    {
                        newRenderPair.startKeyframe = keyframe;
                        newRenderPair.objectId = x;
                        /*TODO: fix property type*/
                        std::cout << "Properties found\n";
                        newRenderPair.propertyType = 1;
                        newRenderPair.propertyName = propertyIterator->name();
                        newRenderPair.startValue = propertyIterator->value();
                        this->renderingData.push_back(newRenderPair);
                    }
                }
            }
        }
        //Reads the keyframe data from an XML file
        void readKeyframesFromFile(std::string filename)
        {
            pugi::xml_parse_result x = this->keyframeData->load_file(filename.c_str());
            std::cout << x.description() << "First child:" << this->keyframeData->first_child().name() << std::endl;
        };
        //Used for printing PseudoGroups
        void displayPseudoGroups()
        {
            for (map<std::string, std::list<string>>::iterator itr = this->pseudoGroups.begin(); itr != this->pseudoGroups.end(); ++itr)
            {
                cout << itr->first << ":" << endl;
                for (auto it2 = itr->second.begin(); it2 != itr->second.end(); ++it2)
                {
                    cout << "\t" << (*it2) << ";" << endl;
                }
            }
        }
        //Creates a PseudoGroup
        void createPseudoGroup(std::string groupName, std::list<std::string> objectIds)
        {
            this->pseudoGroups.insert({groupName, objectIds});
        }
        //Sets the PseudoGroup's property at a specific keyframe with some more properties.
        void setPseudoGroupAtKeyframe(std::string groupName, std::string attributeName, std::string value, unsigned int keyframeNum, int randRangeStart = 0, int randRangeEnd = 0)
        {
            auto it = this->pseudoGroups.find(groupName);
            srand(time(NULL));
            bool diff = false;
            if (randRangeEnd - randRangeStart > 0)
            {
                for (auto objectIt = it->second.begin(); objectIt != it->second.end(); ++objectIt)
                {
                    int randomisedKf = (rand() % (randRangeEnd - randRangeStart)) + randRangeStart;
                    SetObjectAttributeAtKeyframe((*objectIt), attributeName, value, keyframeNum + randomisedKf);
                }
            }
            else
            {
                for (auto objectIt = it->second.begin(); objectIt != it->second.end(); ++objectIt)
                {
                    SetObjectAttributeAtKeyframe((*objectIt), attributeName, value, keyframeNum);
                }
            }
        };
        //Renders
        void render()
        {
            int lastKeyframe, counter;
            calcKeyframePoints();
            lastKeyframe = *--(keyframes.end());
            counter = 0; //keyframes.size();
            for (auto it = keyframes.begin(); it != keyframes.end(); ++it)
            {
                std::cout << endl
                          << "Pointing " << *it;
            }
            std::cout << endl
                      << "Last keyframe is:" << lastKeyframe;
            std::string folderName;
            std::cout << "Enter folder name \n";
            std::cin >> folderName;
            system(("mkdir "+folderName+" && cd "+folderName).c_str());
            for (int currentKeyframe = 0; currentKeyframe != lastKeyframe + 1; currentKeyframe++)
            {
                /*Check if the next major keyframe matches the current keyframe*/
                bool majorKeyframe = false;

                if ((std::find(this->keyframes.begin(), this->keyframes.end(), currentKeyframe) != this->keyframes.end()) && currentKeyframe != lastKeyframe)
                {
                    updateRenderingData(currentKeyframe);
                    std::cout << endl
                              << currentKeyframe;
                    displayRenderingData();
                    majorKeyframe = true;
                    counter += 1;
                }
                for (auto updateSVGIter = this->renderingData.begin(); updateSVGIter != this->renderingData.end(); ++updateSVGIter)
                {
                    pugi::xml_node objectNode = (this->doc->select_node(("//*[@id=\"" + updateSVGIter->objectId + "\"]").c_str()).node());

                    if (objectNode != NULL)
                    {
                        std::cout << endl
                                  << "Reached updating keyframe:" << currentKeyframe << "   " << ("//*[@id='" + updateSVGIter->objectId + "']") << "   " << objectNode.name();
                        if (objectNode.attribute(updateSVGIter->propertyName.c_str()).value() != "")
                        {
                            std::cout << "Reached here before crashing";
                            if (updateSVGIter->propertyName.compare("fill") != 0)
                            {
                                if(updateSVGIter->propertyName.compare("transform") != 0){
                                    objectNode.attribute(updateSVGIter->propertyName.c_str()).set_value(std::to_string(linearInterpolate((*updateSVGIter), currentKeyframe)).c_str());
                                    std::cout << "The updated value is " << std::to_string(linearInterpolate((*updateSVGIter), currentKeyframe)).c_str();
                                }
                                else{
                                    objectNode.attribute(updateSVGIter->propertyName.c_str()).set_value(transformInterpolate((*updateSVGIter), currentKeyframe).c_str());
                                    std::cout << "The updated value is " << transformInterpolate((*updateSVGIter), currentKeyframe).c_str();
                                }
                            }
                            else
                            {
                                std::cout << endl
                                          << colorInterpolate((*updateSVGIter), currentKeyframe) << " One cycle " << endl;
                                objectNode.attribute(updateSVGIter->propertyName.c_str()).set_value(colorInterpolate((*updateSVGIter), currentKeyframe).c_str());
                            }
                        }
                    }
                }

                this->writeSvg(folderName, (std::to_string(currentKeyframe) + ".svg"), addZeroes(currentKeyframe + 1, lastKeyframe + 1) + ".png");

            }
            bool z=true;


        };

    private:
        void batchRender(string folderName){
            //system(("cd "+ folderName+" && ffmpeg -framerate 60 -i \"%03d.png\" output.mp4 " + std::to_string(lastKeyframe)).c_str());
        }
        std::map<std::string, std::list<std::string>> pseudoGroups;
        void writeSvg(std::string dir, std::string svgFilename, std::string outputFilename)
        {
            this->doc->save_file((dir+"/"+svgFilename).c_str());

        };
        int hex2int(char ch)
        {
            if (ch >= '0' && ch <= '9')
                return ch - '0';
            if (ch >= 'A' && ch <= 'F')
                return ch - 'A' + 10;
            if (ch >= 'a' && ch <= 'f')
                return ch - 'a' + 10;
        }
        std::string int2hex(int x)
        {
            char a[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
            char b[3];
            b[0] = a[(x - (x % 16)) / 16];
            b[1] = a[x % 16];
            b[2] = '\0';
            return b;
        }
        std::string transformInterpolate(renderPair rp, int currentKeyframe){
            std::map<std::string, std::list<float>> map1,map2;
            map1=createMap(rp.startValue);
            map2=createMap(rp.endValue);
            return createStringFromMap(interpolateMap(map1, map2, rp.startKeyframe, rp.endKeyframe, currentKeyframe));
        };
        std::map<std::string, std::list<float>> interpolateMap(std::map<std::string, std::list<float>> startMap, std::map<std::string, std::list<float>> endMap, int startKeyframe, int endKeyframe, int currentKeyframe)
        {
            std::map<std::string, std::list<float>> newMap;
            for (auto it = startMap.begin(); it != startMap.end(); ++it)
            {
                if (endMap.find(it->first) != endMap.end())
                {
                    auto endValues = endMap.at(it->first);
                    std::list<float> newValues;
                    auto endValuesIt = endValues.begin();
                    for (auto startValuesIt = it->second.begin(); startValuesIt != it->second.end(); ++startValuesIt)
                    {
                        newValues.push_back(linearInterpolate(*startValuesIt, *endValuesIt, startKeyframe, endKeyframe, currentKeyframe));
                        ++endValuesIt;
                    }
                    newMap.insert({it->first, newValues});
                }
                else
                {
                    newMap.insert(*it);
                }
            }

            for (auto it = endMap.begin(); it != endMap.end(); ++it)
            {
                if (startMap.find(it->first) == startMap.end())
                {
                    newMap.insert(*it);
                }
            return newMap;
            }
        }
        std::string createStringFromMap(std::map<std::string, std::list<float>> dataMap)
        {
            std::string returnString;
            for (auto it = dataMap.begin(); it != dataMap.end(); ++it)
            {
                returnString.append(it->first);
                returnString.append("(");
                for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                {
                    returnString.append(std::to_string(*it2));
                    returnString.append(", ");
                }
                returnString = returnString.substr(0, returnString.size() - 2);
                returnString.append(")");
            }

            return returnString;
        }
        std::string hexInterpolate(renderPair rp, int currentFrame)
        {
            bool hasA;
            int colorType; //1:HEX(native), 2:RGB
            unsigned char colorValsStart[4], colorValsEnd[4];
            int count = 0;
            std::string ret = "#";
            if (rp.startValue.length() == 9)
            {
                hasA = true;
                auto it2 = rp.endValue.begin();
                for (auto it = rp.startValue.begin(); it != rp.startValue.end(); ++it)
                {
                    if (*it != '#')
                    {
                        colorValsStart[count] = hex2int(*it) * 16;
                        colorValsEnd[count] = hex2int(*it2) * 16;

                        it++;
                        it2++;
                        colorValsStart[count] += hex2int(*it);
                        colorValsEnd[count] += hex2int(*it2);
                        count += 1;
                        ret += int2hex(linearInterpolate(colorValsStart[count], colorValsEnd[count], rp.startKeyframe, rp.endKeyframe, currentFrame));
                    }
                    ++it2;
                }
            }
            else
            {
                hasA = false;
                auto it2 = rp.endValue.begin();
                for (auto it = rp.startValue.begin(); it != rp.startValue.end(); ++it)
                {
                    if (*it != '#')
                    {
                        colorValsStart[count] = hex2int(*it) * 16;
                        colorValsEnd[count] = hex2int(*it2) * 16;
                        ++it;
                        ++it2;
                        colorValsStart[count] += hex2int(*it);
                        colorValsEnd[count] += hex2int(*it2);
                        ret += int2hex(linearInterpolate(colorValsStart[count], colorValsEnd[count], rp.startKeyframe, rp.endKeyframe, currentFrame));
                        count += 1;
                    }
                    ++it2;
                }
            }
            std::cout << endl
                      << ret << endl;
            return ret;
        }
        std::string colorInterpolate(renderPair rp, int currentFrame)
        {
            /*bool hasA;
                    int colorType; //1:HEX(native), 2:RGB
                    unsigned char colorValsStart[4], colorValsEnd[4];
                    int count=0;
                    std::string ret="#";*/
            /*HEX implementation*/
            removeRedundantSpaces(&rp.startValue);
            removeRedundantSpaces(&rp.endValue);
            if (*(rp.startValue.begin()) == '#')
            {
            }
            else if (*(rp.startValue.begin()) == 'r' || *(rp.startValue.begin()) == 'R')
            {
                std::string storage, ret = "#";
                int count = 0;
                unsigned char countVals[4];
                bool passedBracket = false;
                for (auto letterIt = rp.startValue.begin(); letterIt != rp.startValue.end(); ++letterIt)
                {
                    if (!passedBracket)
                    {
                        if (isdigit(*letterIt))
                        {
                            passedBracket = true;
                        }
                    }
                    if (isdigit(*letterIt) && passedBracket)
                    {
                        storage += *letterIt;
                    }
                    else if (passedBracket && !isdigit(*letterIt))
                    {
                        countVals[count] = std::stoi(storage);
                        storage = "";
                        count++;
                    }
                }
                for (int i = 0; i != count; i++)
                {
                    ret += int2hex(countVals[i]);
                }
                rp.startValue = ret;
            }
            if (*(rp.endValue.begin()) == '#')
            {
            }
            else if (*(rp.endValue.begin()) == 'r' || *(rp.endValue.begin()) == 'R')
            {
                std::string storage, ret = "#";
                std::cout << rp.endValue;
                int count = 0;
                unsigned char countVals[4];
                bool passedBracket = false;
                for (auto letterIt = rp.endValue.begin(); letterIt != rp.endValue.end(); ++letterIt)
                {
                    if (!passedBracket)
                    {
                        if (isdigit(*letterIt))
                        {
                            passedBracket = true;
                        }
                    }
                    if (isdigit(*letterIt) && passedBracket)
                    {
                        storage += *letterIt;
                    }
                    else if (passedBracket && !isdigit(*letterIt))
                    {
                        printf("Storage: %s \n", storage.c_str());
                        countVals[count] = std::stoi(storage);
                        storage = "";
                        count++;
                    }
                }
                for (int i = 0; i != count; i++)
                {
                    printf("CountVals[ %d ]: %d ", i, countVals[count]);
                    ret += int2hex(countVals[i]);
                }
                rp.endValue = ret;
            }
            printf("The values are: %s and %s \n", rp.startValue.c_str(), rp.endValue.c_str());
            return hexInterpolate(rp, currentFrame);
            /*if(*(rp.startValue.begin())=='#'){
                        colorType=1;
                        if(rp.startValue.length()==9){
                            hasA=true;
                            auto it2=rp.endValue.begin();
                            for(auto it=rp.startValue.begin();it!=rp.startValue.end();++it){


                                if(*it!='#'){
                                    colorValsStart[count]=hex2int(*it)*16;
                                    colorValsEnd[count]=hex2int(*it2)*16;

                                    it++;
                                    it2++;
                                    colorValsStart[count]+=hex2int(*it);
                                    colorValsEnd[count]+=hex2int(*it2);
                                    std::cout << endl << " Important data: " << hex2int(*it) << "   " << hex2int(*it2) << endl;
                                    count+=1;

                                    //std::cout << endl << " Important data: " << *it << "   " << colorValsEnd[count] << endl;//linearInterpolate(colorValsStart[count], colorValsEnd[count], rp.startKeyframe, rp.endKeyframe, currentFrame);// << "    " << int2hex(linearInterpolate(colorValsStart[count], colorValsEnd[count], rp.startKeyframe, rp.endKeyframe, currentFrame)) << endl;
                                    cout << "Checking:" << colorValsStart[count] << "   " << colorValsEnd[count];
                                    ret+=int2hex(linearInterpolate(colorValsStart[count], colorValsEnd[count], rp.startKeyframe, rp.endKeyframe, currentFrame));
                                }
                                ++it2;
                            }
                        }
                        else{
                            hasA=false;
                            auto it2=rp.endValue.begin();
                            for(auto it=rp.startValue.begin();it!=rp.startValue.end();++it){
                                if(*it!='#'){
                                    colorValsStart[count]=hex2int(*it)*16;
                                    colorValsEnd[count]=hex2int(*it2)*16;
                                    printf("Debug logA: %c \n", (*it2));

                                    //printf("The colorVals part 1 is: %d \n", hex2int(*it2)*16);
                                    ++it;
                                    ++it2;
                                    colorValsStart[count]+=hex2int(*it);
                                    //printf("The colorValsEnd is: %d \n", colorValsEnd[count]);
                                    colorValsEnd[count]+=hex2int(*it2);
                                    printf("Debug logB: %c \n", (*it2));
                                    if(count==1){
                                        printf("Debug log: %d \n", colorValsEnd[count]);
                                    }

                                    ret+=int2hex(linearInterpolate(colorValsStart[count], colorValsEnd[count], rp.startKeyframe, rp.endKeyframe, currentFrame));
                                    count+=1;
                                }
                                ++it2;
                            }
                        }
                        std::cout << endl << ret << endl;
                        return ret;
                    }
                    //RGB comparison
                    else if(*(rp.startValue.begin())=='r')
                    {

                    }*/
        };
        void removeRedundantSpaces(std::string *str)
        {
            for (auto it = str->begin(); it != str->end(); ++it)
            {
                if ((*it) == ' ' || (*it) == '\n')
                {
                    it = str->erase(it);
                }
            }
        };
        float linearInterpolate(renderPair rp, int currentFrame)
        {
            //std::cout << (float)(std::stof(rp.startValue)-std::stof(rp.endValue))*((currentFrame-rp.startKeyframe)*1.0/(rp.endKeyframe-rp.startKeyframe))+std::stof(rp.startValue);
            return (float)((std::stof(rp.endValue) - std::stof(rp.startValue)) * ((currentFrame - rp.startKeyframe) * 1.0 / (rp.endKeyframe - rp.startKeyframe)) + std::stof(rp.startValue));
        }
        float linearInterpolate(float startValue, float endValue, int startKeyframe, int endKeyframe, int currentFrame)
        {
            return (float)((endValue - startValue) * ((currentFrame - startKeyframe) * 1.0 / (endKeyframe - startKeyframe)) + startValue);
        }
        pugi::xml_document keyframeData;
        std::list<renderPair> renderingData;
        void displayRenderPair(renderPair p)
        {
            std::cout << "Object ID:" << p.objectId << "Property Name:" << p.propertyName << "Start value:" << p.startValue << "End:" << p.endValue << "Start keyframe:" << p.startKeyframe << "End keyframe:" << p.endKeyframe;
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

        void displayRenderingData()
        {
            std::cout << "Hey there";
            for (auto it = this->renderingData.begin(); it != this->renderingData.end(); ++it)
            {
                std::cout << endl
                          << "Object ID: " << it->objectId << " Property Name: " << it->propertyName << " Start value: " << it->startValue << " End:" << it->endValue << " Start keyframe:" << it->startKeyframe << " End keyframe:" << it->endKeyframe;
            }
        };

    };

#endif
#endif


#ifdef PUGIXML

int main()
{
    std::string fname;
    pugi::xml_document doc;
    std::cout << "Enter filename:\n";
    std::cin >> fname;
    pugi::xml_parse_result result = doc.load_file(fname.c_str());
    /*std::cout << "Load result: " << result.description() << std::endl;*/
    MoPlusPlus::NatronSVGObject svg1;
    svg1.doc = &doc;
    std::cout << svg1.doc->first_child().name() << std::endl;
    /*svg1.SetObjectAttributeAtKeyframe("circleObj", "fill","rgb(0,0,0)", 0, "path");
    svg1.SetObjectAttributeAtKeyframe("circleObj", "fill","rgb(255,255,255)", 5, "path");*/
    int taskMode = 0, keyframe = 0, readFromFile = 0;
    std::cout << "Should I read data from file?1 for Y and 0 for N" << std::endl;
    std::cin >> readFromFile;
    if (readFromFile)
    {
        std::cout << "Enter keyframe data filename:\n";
        std::cin >> fname;
        svg1.readKeyframesFromFile(fname.c_str());
    }
    scanf("%d", &taskMode);
    while (taskMode != -1)
    {
        std::cout  << "Up here";

        if (taskMode == 1)
        {
            cout << "Which keyframe should I jump to?" << endl;
            scanf("%d", &keyframe);
            cout << "Which object do you want to modify?" << endl;
            auto objectList = svg1.GetObjectsList();
            int count = 0;
            for (auto iterator = objectList.begin(); iterator != objectList.end(); ++iterator)
            {
                count += 1;
                cout << count << ">" << iterator->node().attribute("id").value() << endl;
            }
            int objectIdCount;

            cin >> objectIdCount;
            auto i = objectList.begin();
            std::advance(i, objectIdCount - 1);
            std::string objectId = i->node().attribute("id").value(), propertyName;
            cout << "Enter the property name";
            cin >> propertyName;
            std::string value;
            cout << "Enter the value";
            cin >> value;
            svg1.SetObjectAttributeAtKeyframe(objectId, propertyName, value, keyframe);
            std::cout << "Breaking from task mode 1";
        }
        else if (taskMode == 2)
        {
            std::string groupName, propertyName, value;
            std::list<std::string> objects;
            int objectIdCount = 0, keyframe;
            cout << "Enter group name" << endl;
            cin >> groupName;
            while (1)
            {
                auto objectList = svg1.GetObjectsList();
                int count = 0;
                for (auto iterator = objectList.begin(); iterator != objectList.end(); ++iterator)
                {
                    count += 1;
                    cout << count << ">" << iterator->node().attribute("id").value() << endl;
                }
                cin >> objectIdCount;
                if (objectIdCount == -1)
                {
                    svg1.createPseudoGroup(groupName, objects);
                    break;
                }
                auto i = objectList.begin();
                std::advance(i, objectIdCount - 1);
                objects.push_back(std::string(i->node().attribute("id").value()));
            }
            cout << "Enter the keyframe" << endl;
            cin >> keyframe;
            cout << "Enter the property name" << endl;
            cin >> propertyName;
            cout << endl
                 << "Enter the value" << endl;
            cin >> value;
            int randRangeStart, randRangeEnd;
            cout << endl
                 << "Enter random keyframe offset start" << endl;
            cin >> randRangeStart;
            cout << endl
                 << "Enter random keyframe offset end" << endl;
            cin >> randRangeEnd;
            svg1.setPseudoGroupAtKeyframe(groupName, propertyName, value, keyframe, randRangeStart, randRangeEnd);
        }
        else if (taskMode == 3)
        {
            std::string groupName, propertyName, value;
            cout << "Which keyframe should I jump to?" << endl;
            scanf("%d", &keyframe);
            svg1.displayPseudoGroups();
            cout << "Enter group name" << endl;
            cin >> groupName;
            cout << "Enter the property name" << endl;
            cin >> propertyName;
            cout << endl
                 << "Enter the value" << endl;
            cin >> value;
            int randRangeStart, randRangeEnd;
            cout << endl
                 << "Enter random keyframe offset start" << endl;
            cin >> randRangeStart;
            cout << endl
                 << "Enter random keyframe offset end" << endl;
            cin >> randRangeEnd;
            svg1.setPseudoGroupAtKeyframe(groupName, propertyName, value, keyframe, randRangeStart, randRangeEnd);
        }
        scanf("%d", &taskMode);
      }
    svg1.writeKeyframeData("kfData.xml");
    svg1.render();
    doc.save_file("written.svg");
    pugi::xpath_node_set tools = svg1.GetObjectsList();
}
#endif
#ifdef TINYXML
using namespace tinyxml2;
int main()
{
    std::string str;
    std::getline(std::cin, str);
    std::cout << str << std::endl;
    XMLDocument doc;
    //const XMLNode *root=doc.RootElement();
    const char *x = "//*[sA]";
    const TiXmlNode *root = doc.RootElement();
    //TinyXPath::xpath_processor *prc=new TinyXPath::xpath_processor();
    TinyXPath::xpath_processor *prc = new TinyXPath::xpath_processor(root, x);
    doc.LoadFile("test.svg");
    XMLElement *firstChild = doc.FirstChildElement();
    printf(firstChild->Value());
    std::cout << "Done reading" << std::endl;
    //TiXmlNode* pSiteNode = prc.er_compute_xpath(doc.RootElement(), "//*");

    //TIXML_STRING S_res = TinyXPath::S_xpath_string (XDp_doc -> RootElement (), "/office:document-content/office:body/office:text/text:p/text()");

    for (XMLElement *child = doc.FirstChildElement(); child != NULL; child = child->FirstChildElement())
    {
        std::string title = child->Value();
        if (title.compare("path") == 0)
        {
            if (child->Attribute(str.c_str()) == NULL)
            {
                printf("No attribute named %s\n", str);
            }
            else
            {
                cout << "Attribute " << str << "Has value " << child->Attribute(str.c_str()) << endl;
            }
        }
        cout << title << endl;
    }
    return 0;
}
#endif

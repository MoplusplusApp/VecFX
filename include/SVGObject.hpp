#include <iostream>
#include <string>
#include <cmath>
#include <map>
#include <iterator>
#include <list>
#include <algorithm>
#include <time.h>
#include "pugixml.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>



    int randomNumInRange(int rangeStart, int rangeEnd);
    std::string addZeroes(int number, int maxNumber);
    std::map<std::string, int> interpolationMethod;
    float linearInterpolate(float start, float end, int startFrame, int endFrame, int currentFrame);
    class NatronSVGObject
    {
    public:
        struct renderPair;
        pugi::xpath_node_set GetObjectsList();
        void SetObjectAttributeAtKeyframe(std::string objectId, std::string attributeName, std::string value, unsigned int keyframeNum, std::string elementType = "*");
        void writeKeyframeData(std::string filename);
        void calcKeyframePoints();
        renderPair findNextReference(pugi::xml_node_iterator currentKeyframeNodeIterator, std::string objectId, std::string propertyName, std::string prevValue);
        void updateRenderingData(int keyframe);
        void readKeyframesFromFile(std::string filename);
        void displayPseudoGroups();
        void createPseudoGroup(std::string groupName, std::list<std::string> objectIds);
        void setPseudoGroupAtKeyframe(std::string groupName, std::string attributeName, std::string value, unsigned int keyframeNum, int randRangeStart = 0, int randRangeEnd = 0);
        void render();
    private:
        void batchRender(std::string folderName);
        void writeSvg(std::string dir, std::string svgFilename, std::string outputFilename);
        int hex2int(char ch);
        std::string int2hex(int x);
        std::string transformInterpolate(renderPair rp, int currentKeyframe);
        std::map<std::string, std::list<float>> interpolateMap(std::map<std::string, std::list<float>> startMap, std::map<std::string, std::list<float>> endMap, int startKeyframe, int endKeyframe, int currentKeyframe);
        std::string createStringFromMap(std::map<std::string, std::list<float>> dataMap);
        std::string hexInterpolate(renderPair rp, int currentFrame);
        std::string colorInterpolate(renderPair rp, int currentFrame);
        void removeRedundantSpaces(std::string *str);
        float linearInterpolate(renderPair rp, int currentFrame);
        void displayRenderPair(renderPair p);
        std::map<std::string, std::list<float>> createMap(std::string value);
        void displayRenderingData();

    };

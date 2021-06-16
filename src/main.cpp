#include "SVGObject.hpp"
#include "pugixml.hpp"
#include <stdio.h>



int main()
{
    std::string fname;
    pugi::xml_document doc;
    std::cout << "Enter filename:\n";
    std::cin >> fname;
    pugi::xml_parse_result result = doc.load_file(fname.c_str());
    /*std::cout << "Load result: " << result.description() << std::endl;*/
    NatronSVGObject svg1;
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
            std::cout << "Which keyframe should I jump to?" << std::endl;
            scanf("%d", &keyframe);
            std::cout << "Which object do you want to modify?" << std::endl;
            auto objectList = svg1.GetObjectsList();
            int count = 0;
            for (auto iterator = objectList.begin(); iterator != objectList.end(); ++iterator)
            {
                count += 1;
                std::cout << count << ">" << iterator->node().attribute("id").value() << std::endl;
            }
            int objectIdCount;

            std::cin >> objectIdCount;
            auto i = objectList.begin();
            std::advance(i, objectIdCount - 1);
            std::string objectId = i->node().attribute("id").value(), propertyName;
            std::cout << "Enter the property name";
            std::cin >> propertyName;
            std::string value;
            std::cout << "Enter the value";
            std::cin >> value;
            svg1.SetObjectAttributeAtKeyframe(objectId, propertyName, value, keyframe);
            std::cout << "Breaking from task mode 1";
        }
        else if (taskMode == 2)
        {
            std::string groupName, propertyName, value;
            std::list<std::string> objects;
            int objectIdCount = 0, keyframe;
            std::cout << "Enter group name" << std::endl;
            std::cin >> groupName;
            while (1)
            {
                auto objectList = svg1.GetObjectsList();
                int count = 0;
                for (auto iterator = objectList.begin(); iterator != objectList.end(); ++iterator)
                {
                    count += 1;
                    std::cout << count << ">" << iterator->node().attribute("id").value() << std::endl;
                }
                std::cin >> objectIdCount;
                if (objectIdCount == -1)
                {
                    svg1.createPseudoGroup(groupName, objects);
                    break;
                }
                auto i = objectList.begin();
                std::advance(i, objectIdCount - 1);
                objects.push_back(std::string(i->node().attribute("id").value()));
            }
            std::cout << "Enter the keyframe" << std::endl;
            std::cin >> keyframe;
            std::cout << "Enter the property name" << std::endl;
            std::cin >> propertyName;
            std::cout << std::endl
                 << "Enter the value" << std::endl;
            std::cin >> value;
            int randRangeStart, randRangeEnd;
            std::cout << std::endl
                 << "Enter random keyframe offset start" << std::endl;
            std::cin >> randRangeStart;
            std::cout << std::endl
                 << "Enter random keyframe offset end" << std::endl;
            std::cin >> randRangeEnd;
            svg1.setPseudoGroupAtKeyframe(groupName, propertyName, value, keyframe, randRangeStart, randRangeEnd);
        }
        else if (taskMode == 3)
        {
            std::string groupName, propertyName, value;
            std::cout << "Which keyframe should I jump to?" << std::endl;
            scanf("%d", &keyframe);
            svg1.displayPseudoGroups();
            std::cout << "Enter group name" << std::endl;
            std::cin >> groupName;
            std::cout << "Enter the property name" << std::endl;
            std::cin >> propertyName;
            std::cout << std::endl
                 << "Enter the value" << std::endl;
            std::cin >> value;
            int randRangeStart, randRangeEnd;
            std::cout << std::endl
                 << "Enter random keyframe offset start" << std::endl;
            std::cin >> randRangeStart;
            std::cout << std::endl
                 << "Enter random keyframe offset end" << std::endl;
            std::cin >> randRangeEnd;
            svg1.setPseudoGroupAtKeyframe(groupName, propertyName, value, keyframe, randRangeStart, randRangeEnd);
        }
        scanf("%d", &taskMode);
      }
    svg1.writeKeyframeData("kfData.xml");
    svg1.render();
    doc.save_file("written.svg");
    pugi::xpath_node_set tools = svg1.GetObjectsList();
}
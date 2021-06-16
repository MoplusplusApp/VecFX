#include "SVGObject.hpp"
#include "pugixml.hpp"
#include <stdio.h>
#include <string>
#include <functional>

using namespace std;

// Functions

// Input validation function
bool check_answer(string& answer, bool& result)
{
    transform(answer.begin(), answer.end(), answer.begin(),
                   [](unsigned char x){return ::tolower(x);});

     bool answer_valid =
            (answer == "y")   ||
            (answer == "n")   ||
            (answer == "yes") ||
            (answer == "no");

    result = answer_valid && answer[0] == 'y';    
    return answer_valid;
}


// Check yes or no function
// Example usage:
//    question_yes_no("Do you want to continue?")
//    --> returns true or false
bool question_yes_no(string const& message)
{
    string answer;
    bool result;
    cout << message << "\n[Y/N]: ";
    while(cin >> answer && !check_answer(answer, result))
    {
        cout << "Invalid answer: " << answer << " Please try again\n"
                  << message << "? [Y/n]\n";
    }
    if (!cin) {
        // We never got an answer, so we'll just give up
        throw runtime_error("User input read failed!");
    }
    return result;
}

// Tutorial
int tutorial(void) {
	cout << "Sorry, the tutorial feature is still WIP. Exiting...\n";
	exit(0);
}

// Main function for the application
int mainprocess(void) {
	string fname;
	pugi::xml_document doc;
	cout << "Enter filename:\n";
	cin >> fname;
	pugi::xml_parse_result result = doc.load_file(fname.c_str());
	/*cout << "Load result: " << result.description() << endl;*/
	NatronSVGObject svg1;
	svg1.doc = &doc;
	cout << svg1.doc->first_child().name() << endl;
	/*svg1.SetObjectAttributeAtKeyframe("circleObj", "fill","rgb(0,0,0)", 0, "path");
	svg1.SetObjectAttributeAtKeyframe("circleObj", "fill","rgb(255,255,255)", 5, "path");*/
	int taskMode = 0, keyframe = 0, readFromFile = 0;
	cout << "Should I read data from file? 1 for Y and 0 for N" << endl;
	cin >> readFromFile;
	if (readFromFile)
	{
	    cout << "Enter keyframe data filename:\n";
	    cin >> fname;
	    svg1.readKeyframesFromFile(fname.c_str());
	}
	scanf("%d", &taskMode);
	while (taskMode != -1)
	{
	    cout  << "Up here";

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
	        advance(i, objectIdCount - 1);
	        string objectId = i->node().attribute("id").value(), propertyName;
	        cout << "Enter the property name";
	        cin >> propertyName;
	        string value;
	        cout << "Enter the value";
	        cin >> value;
	        svg1.SetObjectAttributeAtKeyframe(objectId, propertyName, value, keyframe);
	        cout << "Breaking from task mode 1";
	    }
	    else if (taskMode == 2)
	    {
	        string groupName, propertyName, value;
	        list<string> objects;
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
	            advance(i, objectIdCount - 1);
	            objects.push_back(string(i->node().attribute("id").value()));
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
	        string groupName, propertyName, value;
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
	return 0;
}

int main(int argc, char **argv)
{
    cout << "Welcome to VecFX standalone version 0.1.0.\n";
	cout << "Run VecFX with the -t flag for a tutorial, or -r to start immediately\n\n";
	// Loop through array of arguments
	// - Less than 1 arg (no args) - show message then exit
	// - Exactly 1 arg - parse arg
	// - Greater than 1 arg - ask user what to do
	// - Else: ask user what to do
	if (argc < 2) {
		// We just show the help message if there are no arguments provided
		// and then exit
		cout << "You provided no arguments. Exiting...\n";
		exit(0);
	}
	else if (argc == 2) {
		for (int i = 1; i < argc; i++) {
			if (string(argv[i]) == "-r") {
				// Run the main function
				mainprocess();
			}
			else if (string(argv[i]) == "-t") {
				// Start the tutorial
				tutorial();
			}
			else {
				cout << "We can't quite recognize the argument you provided\n";
				if (question_yes_no("Do you want to run anyway?") == true) {
					mainprocess();
				} else {
					cout << "Exiting...\n";
					exit(0);
				}
			}
		}
	}
	else if (argc > 2) {
		cout << "Sorry, you can only use one argument at the moment\n";
		// Intelligently guess the correct argument
		int lastnum = argc - 1;
		string last_argument = string(argv[lastnum]);
		string question =
			string("Did you mean to use the ")
			+ last_argument
			+ string(" argument?");
		if (question_yes_no(question) == true) {
			// Ask user what to do
			if (last_argument == "-r") {
				// Run the main function
				mainprocess();
			}
			else if (last_argument == "-t") {
				// Start the tutorial
				tutorial();
			}
			else {
				cout << "We can't quite recognize the argument you provided\n";
				if (question_yes_no("Do you want to run anyway?") == true) {
					mainprocess();
				} else {
					cout << "Exiting...\n";
					exit(0);
				}
			}
		} else {
			cout << "Exiting...\n";
			exit(0);
		}
	}
}

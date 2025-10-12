#include "Cryption.hpp"
#include  "../Procesess/Task.hpp"
#include "../fileHanding/ReadENV.cpp"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
using namespace std;


// Emoji mapping for encryption - 256 different emojis
vector<string> getEmojiMap() {
    return {
        "😀", "😃", "😄", "😁", "😆", "😅", "😂", "🤣", "😊", "😇", "🙂", "🙃", "😉", "😌", "😍", "🥰",
        "😘", "😗", "😙", "😚", "😋", "😛", "😝", "😜", "🤪", "🤨", "🧐", "🤓", "😎", "🤩", "🥳", "😏",
        "😒", "😞", "😔", "😟", "😕", "🙁", "😣", "😖", "😫", "😩", "🥺", "😢", "😭", "😤", "😠", "😡",
        "🤬", "🤯", "😳", "🥵", "🥶", "😱", "😨", "😰", "😥", "😓", "🤗", "🤔", "🤭", "🤫", "🤥", "😶",
        "😐", "😑", "😬", "🙄", "😯", "😦", "😧", "😮", "😲", "🥱", "😴", "🤤", "😪", "😵", "🤐", "🥴",
        "🤢", "🤮", "🤧", "😷", "🤒", "🤕", "🤑", "🤠", "😈", "👿", "👹", "👺", "🤡", "💩", "👻", "💀",
        "👽", "👾", "🤖", "🎃", "😺", "😸", "😹", "😻", "😼", "😽", "🙀", "😿", "😾", "👶", "👧", "🧒",
        "👦", "👩", "🧑", "👨", "👵", "🧓", "👴", "👲", "👳", "🧕", "👮", "👷", "💂", "🕵", "👩‍⚕️", "👨‍⚕️",
        "👩‍🌾", "👨‍🌾", "👩‍🍳", "👨‍🍳", "👩‍🎓", "👨‍🎓", "👩‍🎤", "👨‍🎤", "👩‍🏫", "👨‍🏫", "👩‍🏭", "👨‍🏭", "👩‍💻", "👨‍💻", "👩‍💼", "👨‍💼",
        "👩‍🔧", "👨‍🔧", "👩‍🔬", "👨‍🔬", "👩‍🎨", "👨‍🎨", "👩‍🚒", "👨‍🚒", "👩‍✈️", "👨‍✈️", "👩‍🚀", "👨‍🚀", "👩‍⚖️", "👨‍⚖️", "👰", "🤵",
        "👸", "🤴", "🦸", "🦹", "🧙", "🧚", "🧛", "🧜", "🧝", "🧞", "🧟", "💆", "💇", "🚶", "🏃", "💃",
        "🕺", "🕴", "👯", "🧖", "🧗", "🤺", "🏇", "⛷", "🏂", "🏌", "🏄", "🚣", "🏊", "⛹", "🏋", "🚴",
        "🤸", "🤼", "🤽", "🤾", "🤹", "🧘", "🛀", "🛌", "👭", "👫", "👬", "💏", "💑", "👪", "👤", "👥",
        "👣", "🦰", "🦱", "🦳", "🦲", "🐵", "🐒", "🦍", "🦧", "🐶", "🐕", "🦮", "🐕‍🦺", "🐩", "🐺", "🦊",
        "🦝", "🐱", "🐈", "🦁", "🐯", "🐅", "🐆", "🐴", "🐎", "🦄", "🦓", "🦌", "🐮", "🐂", "🐃", "🐄",
        "🐷", "🐖", "🐗", "🐽", "🐏", "🐑", "🐐", "🐪", "🐫", "🦙", "🦒", "🐘", "🦏", "🦛", "🐭", "🐁"
    };
}

// Create reverse mapping for decryption
unordered_map<string, int> createEmojiToValueMap() {
    vector<string> emojis = getEmojiMap();
    unordered_map<string, int> emojiToValue;
    for (size_t i = 0; i < emojis.size(); i++) {
        emojiToValue[emojis[i]] = static_cast<int>(i);
    }
    return emojiToValue;
}

int executeCryption(const string &taskData) {
    Task task=Task::fromString(taskData);
    ReadEnv env;
    string envKey=env.getEnv();
    int key=stoi(envKey);
    
    vector<string> emojiMap = getEmojiMap();
    cout << "Emoji map size: " << emojiMap.size() << endl;

   if(task.action==Action::ENCRYPT){
      // Read the entire file content first
      string content;
      char ch;
      while(task.f_stream.get(ch)){
          content += ch;
      }
      task.f_stream.close();
      
      // Reopen file for writing
      string filename = task.filePath;
      ofstream outFile(filename);
      
      // Encrypt each character and convert to emoji
      for(char c : content) {
          int encrypted = (static_cast<unsigned char>(c) + key) % 256;
          cout << "Character: '" << c << "' (ASCII: " << static_cast<int>(static_cast<unsigned char>(c)) << ") -> encrypted index: " << encrypted << endl;
          
          if(encrypted >= 0 && encrypted < static_cast<int>(emojiMap.size())) {
              outFile << emojiMap[encrypted];
          } else {
              cout << "ERROR: Index " << encrypted << " is out of bounds for emoji map of size " << emojiMap.size() << endl;
          }
      }
      outFile.close();
   }
   else {
       // For decryption, we need to read emojis and convert back
       task.f_stream.close();
       
       // Read the entire file as string to parse emojis
       ifstream inFile(task.filePath);
       string content((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
       inFile.close();
       
       // Create emoji to value mapping
       unordered_map<string, int> emojiToValue = createEmojiToValueMap();
       
       // Open file for writing decrypted content
       ofstream outFile(task.filePath);
       
       // Parse emojis and decrypt
       for(size_t i = 0; i < content.length(); ) {
           // Try to find an emoji starting at position i
           bool found = false;
           for(const auto& pair : emojiToValue) {
               const string& emoji = pair.first;
               if(content.substr(i, emoji.length()) == emoji) {
                   int encryptedValue = pair.second;
                   char decrypted = (encryptedValue - key + 256) % 256;
                   outFile << decrypted;
                   i += emoji.length();
                   found = true;
                   break;
               }
           }
           if(!found) {
               i++; // Skip unknown character
           }
       }
       outFile.close();
   }

   return 0;
}

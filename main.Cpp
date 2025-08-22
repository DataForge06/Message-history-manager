#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <chrono>
#include <ctime>
#include <iomanip>

struct Message {
    std::string content;
    std::chrono::system_clock::time_point timestamp;
};

class HistoryManager {
private:
    std::vector<Message> messages;
    std::stack<Message> undoStack;

public:
    void sendMessage(const std::string& text) {
        Message msg = {text, std::chrono::system_clock::now()};
        messages.push_back(msg);
        std::cout << "Send ho gaya: \"" << text << "\"\n";

        while (!undoStack.empty()) {
            undoStack.pop();
        }
    }

    void undoLastMessage() {
        if (messages.empty()) {
            std::cout << "Message nahi hai undo ke liye.\n";
            return;
        }
        Message lastMessage = messages.back();
        messages.pop_back();
        undoStack.push(lastMessage);
        std::cout << "Undone: \"" << lastMessage.content << "\"\n";
    }

    void redoLastUndoneMessage() {
        if (undoStack.empty()) {
            std::cout << "No message to redo.\n";
            return;
        }
        Message messageToRedo = undoStack.top();
        undoStack.pop();
        messages.push_back(messageToRedo);
        std::cout << "Redone: \"" << messageToRedo.content << "\"\n";
    }

    void displayHistory() const {
        std::cout << "\n----- Chat History -----\n";
        if (messages.empty()) {
            std::cout << "(No messages)\n";
        } else {
            for (const auto& msg : messages) {
                std::time_t time = std::chrono::system_clock::to_time_t(msg.timestamp);
                std::tm local_tm = *std::localtime(&time);
                std::cout << "[" << std::put_time(&local_tm, "%H:%M:%S") << "] " 
                          << msg.content << std::endl;
            }
        }
        std::cout << "-------------------------\n";
    }
};

int main() {
    HistoryManager manager;
    manager.sendMessage("Hello there!");
    manager.sendMessage("How are you?");
    manager.displayHistory();

    std::cout << "\n---- Undoing last message ----\n";
    manager.undoLastMessage();
    manager.displayHistory();

    std::cout << "\n---- Redoing last undone message ----\n";
    manager.redoLastUndoneMessage();
    manager.displayHistory();

    std::cout << "\n---- Trying to redo again (could be able to fail) ----\n";
    manager.redoLastUndoneMessage();
    manager.displayHistory();

    std::cout << "\n---- Trying to redo again (might fail or successful) ----\n";
    manager.redoLastUndoneMessage();
    manager.displayHistory();

    return 0;
}

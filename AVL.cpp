#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;

struct AVLNode {
    int key;
    AVLNode* left;
    AVLNode* right;
    int height;
    AVLNode(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

int height(AVLNode* node) {
    return node ? node->height : 0;
}

int balanceFactor(AVLNode* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

int maxInt(int a, int b) {
    return (a > b) ? a : b;
}

AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = maxInt(height(y->left), height(y->right)) + 1;
    x->height = maxInt(height(x->left), height(x->right)) + 1;
    return x;
}

AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = maxInt(height(x->left), height(x->right)) + 1;
    y->height = maxInt(height(y->left), height(y->right)) + 1;
    return y;
}

AVLNode* insertNode(AVLNode* node, int key) {
    if (!node) return new AVLNode(key);
    if (key < node->key)
        node->left = insertNode(node->left, key);
    else if (key > node->key)
        node->right = insertNode(node->right, key);
    else
        return node;

    node->height = 1 + maxInt(height(node->left), height(node->right));

    int bf = balanceFactor(node);

    if (bf > 1 && key < node->left->key)
        return rightRotate(node);
    if (bf < -1 && key > node->right->key)
        return leftRotate(node);
    if (bf > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (bf < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

AVLNode* minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current && current->left)
        current = current->left;
    return current;
}

AVLNode* deleteNode(AVLNode* root, int key) {
    if (!root) return root;
    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        if (!root->left || !root->right) {
            AVLNode* temp = root->left ? root->left : root->right;
            if (!temp) {
                temp = root;
                root = nullptr;
            }
            else
                *root = *temp;
            delete temp;
        }
        else {
            AVLNode* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }
    if (!root) return root;

    root->height = 1 + maxInt(height(root->left), height(root->right));
    int bf = balanceFactor(root);

    if (bf > 1 && balanceFactor(root->left) >= 0)
        return rightRotate(root);
    if (bf > 1 && balanceFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (bf < -1 && balanceFactor(root->right) <= 0)
        return leftRotate(root);
    if (bf < -1 && balanceFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

// For layout: assign x,y positions to nodes based on inorder traversal

struct NodePosition {
    AVLNode* node;
    float x, y;
};

void inorderPositions(AVLNode* root, vector<NodePosition>& positions, float y, float xSpacing, float& currentX) {
    if (!root) return;
    inorderPositions(root->left, positions, y + 80, xSpacing, currentX);
    positions.push_back({ root, currentX, y });
    currentX += xSpacing;
    inorderPositions(root->right, positions, y + 80, xSpacing, currentX);
}

// ... (Keep all AVL tree code same)

int main() {
    AVLNode* root = nullptr;

    sf::RenderWindow window(sf::VideoMode(1000, 700), "AVL Tree Visualizer");
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cout << "Error loading font\n";
        return -1;
    }

    const float nodeRadius = 25;
    const float xSpacing = 60;

    // Create a view for scrolling/panning
    sf::View view = window.getDefaultView();
    const float panSpeed = 20.f; // pixels per key press

    int choice = 0;
    bool waitingInput = false;
    string inputStr = "";
    enum State { MENU, INPUT_INSERT, INPUT_DELETE } state = MENU;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered && waitingInput) {
                if (event.text.unicode >= '0' && event.text.unicode <= '9') {
                    inputStr.push_back(static_cast<char>(event.text.unicode));
                }
                else if (event.text.unicode == 8) { // backspace
                    if (!inputStr.empty())
                        inputStr.pop_back();
                }
                else if (event.text.unicode == 13) { // enter
                    if (!inputStr.empty()) {
                        int val = stoi(inputStr);
                        if (state == INPUT_INSERT) root = insertNode(root, val);
                        else if (state == INPUT_DELETE) root = deleteNode(root, val);
                    }
                    inputStr.clear();
                    waitingInput = false;
                    state = MENU;
                }
            }

            if (!waitingInput && event.type == sf::Event::KeyPressed) {
                if (state == MENU) {
                    if (event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::I) {
                        state = INPUT_INSERT;
                        waitingInput = true;
                    }
                    else if (event.key.code == sf::Keyboard::Num2 || event.key.code == sf::Keyboard::D) {
                        state = INPUT_DELETE;
                        waitingInput = true;
                    }
                    else if (event.key.code == sf::Keyboard::Num3 || event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                }
                // Handle panning with arrow keys
                if (!waitingInput) {
                    if (event.key.code == sf::Keyboard::Left) {
                        view.move(-panSpeed, 0);
                    }
                    else if (event.key.code == sf::Keyboard::Right) {
                        view.move(panSpeed, 0);
                    }
                    else if (event.key.code == sf::Keyboard::Up) {
                        view.move(0, -panSpeed);
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        view.move(0, panSpeed);
                    }
                }
            }
        }

        window.clear(sf::Color::Black);

        window.setView(view);

        // Draw menu or input prompt (fix position relative to view)
        if (state == MENU) {
            sf::Text title("AVL Tree Visualizer", font, 32);
            title.setFillColor(sf::Color::Green);
            title.setPosition(view.getCenter().x - view.getSize().x / 2 + 300, view.getCenter().y - view.getSize().y / 2 + 10);
            window.draw(title);

            sf::Text menu1("1. Insert (I)", font, 24);
            menu1.setPosition(view.getCenter().x - view.getSize().x / 2 + 20, view.getCenter().y - view.getSize().y / 2 + 60);
            window.draw(menu1);

            sf::Text menu2("2. Delete (D)", font, 24);
            menu2.setPosition(view.getCenter().x - view.getSize().x / 2 + 20, view.getCenter().y - view.getSize().y / 2 + 100);
            window.draw(menu2);

            sf::Text menu3("3. Exit (Esc)", font, 24);
            menu3.setPosition(view.getCenter().x - view.getSize().x / 2 + 20, view.getCenter().y - view.getSize().y / 2 + 140);
            window.draw(menu3);
        }
        else {
            string prompt = (state == INPUT_INSERT) ? "Insert value: " : "Delete value: ";
            sf::Text promptText(prompt + inputStr, font, 28);
            promptText.setPosition(view.getCenter().x - view.getSize().x / 2 + 20, view.getCenter().y - view.getSize().y / 2 + 60);
            promptText.setFillColor(sf::Color::Yellow);
            window.draw(promptText);
        }

        vector<NodePosition> positions;
        float currentX = 70;
        inorderPositions(root, positions, 250, xSpacing, currentX);

        // Draw edges
        for (auto& np : positions) {
            AVLNode* n = np.node;
            if (n->left) {
                for (auto& cnp : positions) {
                    if (cnp.node == n->left) {
                        sf::Vertex line[] = {
                            sf::Vertex(sf::Vector2f(np.x, np.y + nodeRadius), sf::Color::White),
                            sf::Vertex(sf::Vector2f(cnp.x, cnp.y - nodeRadius), sf::Color::White)
                        };
                        window.draw(line, 2, sf::Lines);
                        break;
                    }
                }
            }
            if (n->right) {
                for (auto& cnp : positions) {
                    if (cnp.node == n->right) {
                        sf::Vertex line[] = {
                            sf::Vertex(sf::Vector2f(np.x, np.y + nodeRadius), sf::Color::White),
                            sf::Vertex(sf::Vector2f(cnp.x, cnp.y - nodeRadius), sf::Color::White)
                        };
                        window.draw(line, 2, sf::Lines);
                        break;
                    }
                }
            }
        }

        // Draw nodes
        for (auto& np : positions) {
            sf::CircleShape circle(nodeRadius);
            circle.setFillColor(sf::Color(70, 130, 180));
            circle.setOutlineColor(sf::Color::White);
            circle.setOutlineThickness(2);
            circle.setOrigin(nodeRadius, nodeRadius);
            circle.setPosition(np.x, np.y);
            window.draw(circle);

            sf::Text keyText(to_string(np.node->key), font, 20);
            keyText.setFillColor(sf::Color::White);
            sf::FloatRect textRect = keyText.getLocalBounds();
            keyText.setOrigin(textRect.left + textRect.width / 2.0f,
                textRect.top + textRect.height / 2.0f);
            keyText.setPosition(np.x, np.y);
            window.draw(keyText);
        }

        window.display();
    }

    return 0;
}
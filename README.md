# AVL Tree Visualizer (C++ with SFML)

This project is an **AVL Tree Visualizer** built using **C++** and **SFML** in **Visual Studio**. It demonstrates how AVL trees maintain balance during insertions by visually updating the structure in real-time.



---

## 🧠 Features

- Visual representation of AVL Tree insertions
- Auto-balancing logic with left/right rotations
- Real-time rendering using SFML
- Node animation and graphical links between nodes
- Simple and intuitive layout

---

## 📁 File Structure

avl-tree-visualizer/
├── avl.cpp # Main C++ file with all AVL logic and SFML code
├── README.md # This file

---

## ⚙️ Requirements

- **Visual Studio** (tested with 2019/2022)
- **SFML 2.5.x** or higher installed on your system

### Install SFML for Visual Studio

1. Download SFML from [https://www.sfml-dev.org/download.php](https://www.sfml-dev.org/download.php)
2. Extract and place the folder (e.g., `SFML-2.5.1`) somewhere accessible
3. In your Visual Studio project:
   - Go to **Project Properties**
   - Under **C/C++ → General → Additional Include Directories**:
     - Add: `path\to\SFML\include`
   - Under **Linker → General → Additional Library Directories**:
     - Add: `path\to\SFML\lib`
   - Under **Linker → Input → Additional Dependencies**:
     - Add:
       ```
       sfml-graphics.lib
       sfml-window.lib
       sfml-system.lib
       ```
   - Copy the required `.dll` files from the `SFML/bin` folder to your `Debug` or `Release` output directory

---

## 🛠️ How to Build & Run

1. Open Visual Studio
2. Create a new **Empty C++ Project**
3. Add `avl.cpp` to your project
4. Configure SFML (see instructions above)
5. Build and Run

---

## 💡 Usage

- On running the app, insert values using mouse/keyboard interaction (if implemented)
- The AVL Tree updates in real-time showing rotations and balance factors
- Modify `avl.cpp` to experiment with input sequences or extend functionality

---

## 📷 Preview

![image](https://github.com/user-attachments/assets/0cef1179-6f49-43e5-a444-ff0ed5341648)


---

## 📜 License

This project is open-source and licensed under the [MIT License](LICENSE).

---

## 🤝 Credits

- Developed by Usman
- Built using [SFML](https://www.sfml-dev.org/) and C++

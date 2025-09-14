# Personal Projects Portfolio

This repo collects my key projects across web development, embedded systems, concurrency, and machine learning.  
Each project lives in its own folder with code + a dedicated README.

---

## Projects


### Bank Marketing Prediction – Machine Learning Model (Python, Scikit-learn, Pandas, Seaborn)
- Developed an end-to-end ML pipeline on a dataset of 45,000+ records to predict client term deposit subscriptions.  
- Implemented preprocessing with label/one-hot encoding, stratified splits, and scaling to prepare features.  
- Trained and compared baseline logistic regression, regularized (L1/L2) models, and KNN with hyperparameter tuning.  
- Evaluated performance using precision, recall, F1, ROC AUC, and PR AUC, identifying logistic regression as superior for imbalanced classification.  

---

### Minesweeper AI (Python, Pygame, Knowledge-Based AI)  
- Implemented an AI agent that plays Minesweeper using logical inference and dynamic knowledge representation.  
- Encoded reasoning with Sentence objects, enabling the AI to deduce safe moves and mines from board state.  
- Built an interactive GUI with Pygame supporting manual and AI-assisted gameplay.  
- Applied object-oriented design to separate game logic, AI reasoning, and knowledge base management.  

---

### To-Do Application with GUI (C++, SplashKit, OOP, File Persistence)  
- Developed a desktop task manager with classes for tasks, task lists, menus, and screens using inheritance and polymorphism.  
- Applied dynamic memory management with custom dynamic arrays and pointer-based structures.  
- Designed an interactive GUI (hover effects, clickable tasks, multi-screen navigation).  
- Integrated file persistence (`<fstream>`) for saving and loading tasks with categories, due dates, and completion status.  

---

### Enhanced Reaction-Timer Controller (C#, OOP, State Design Pattern)  
- Designed a reaction timer game using the State Design Pattern with modular states (Idle, Ready, Waiting, Running, Result, FinalResult).  
- Extended base functionality to include multi-trial scoring, cheating detection, and timeout handling.  
- Applied event-driven programming and built a custom test harness (58 scenarios) with mock objects, reflecting TDD practices.  

---

### Automated Parking Toll System (Arduino, IoT, Hardware-Software Integration)  
- Built an Arduino-based vehicle access control system with ultrasonic sensor, servo motor, LEDs, and buzzer.  
- Integrated with Linux terminal (Minicom) via serial communication for real-time monitoring and control.  
- Implemented event-driven logic for detection, access approval/denial, logging, and safety delays.  
- Iteratively enhanced system reliability with traffic light indicators, auditory alerts, and persistent vehicle logs.  

---

### K-Means Clustering – Sequential vs Parallel (C++, OpenMP, Concurrency)  
- Implemented K-Means clustering in C++ with CSV I/O, benchmarking sequential and parallel versions.  
- Parallelized point-to-centroid assignment and centroid updates using OpenMP with static scheduling and reductions.  
- Evaluated performance on a 10-core system: showed limited gains on small datasets due to threading overhead.  
- Demonstrated skills in parallel computing, concurrency trade-offs, and performance analysis.  

---

### Wushu Online – Tutorial Web Platform (Flask, Python, SQLite, HTML/CSS)
- Built a full-stack web application for martial arts tutorials using Flask and SQLite with CS50’s SQL library.  
- Implemented user authentication and session management, restricting access to tutorials until login/registration.  
- Designed a responsive HTML/CSS frontend with dynamic navigation bar, hover animations, and multiple content pages.  
- Integrated a YouTube-based tutorial library, simulating a real-world educational platform.  

---


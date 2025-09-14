# Enhanced Reaction-Timer Controller

This project implements a **reaction timer game** in C#, enhanced with additional features such as multi-trial scoring, cheating detection, and timeout handling. The design follows the **State Design Pattern**, ensuring modularity and extensibility.

---

## Objectives
- Build an **event-driven reaction timer game** with multiple states.  
- Apply **object-oriented principles** (inheritance, polymorphism, encapsulation).  
- Enhance game functionality beyond the basic requirements.  
- Demonstrate **test-driven development (TDD)** with automated test scenarios.  

---

## System Design

### State Design Pattern
The application transitions between modular states:
1. **Idle** – waiting for user input.  
2. **Ready** – preparing for random delay.  
3. **Waiting** – unpredictable wait period before signal.  
4. **Running** – timer running; user must react quickly.  
5. **Result** – show single trial result.  
6. **FinalResult** – display final score across trials.  

### Enhancements
- **Multi-trial scoring**: Tracks user performance over multiple rounds.  
- **Cheating detection**: Identifies if the user presses early during the waiting period.  
- **Timeout handling**: Detects when no response is given within the limit.  

---

## Implementation Highlights
- Designed modular states as **independent classes** implementing a common `IState` interface.  
- Applied **event-driven programming** to handle user interactions and state transitions.  
- Extended base functionality with additional feedback (e.g., “Too Early!”, “Timed Out”).  
- Developed a **custom test harness** with 58 scenarios using **mock objects**, ensuring reliability and adherence to TDD practices.  

---

## Example Flow
1. Game starts in **Idle** state.  
2. User presses start → transitions to **Ready**.  
3. Random delay enters **Waiting** state.  
4. Signal triggers → **Running** state begins.  
5. User reacts → system measures response time.  
6. Results are shown; game moves to **Result** or **FinalResult** after trials.  

---

## Tech Stack
- **Language:** C#  
- **Framework:** .NET / Mono  
- **Concepts:** State Design Pattern, Event-driven programming, TDD  

---

## Run Locally
Clone the repo and build the project:  
```bash
git clone https://github.com/<your-username>/Personal-Projects.git
cd Personal-Projects/Reaction-Timer-Controller

dotnet build
dotnet run

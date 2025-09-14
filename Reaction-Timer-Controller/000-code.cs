using System.Data;
using System.Timers;

namespace SimpleReactionMachine{
    public class EnhancedReactionController : IController{
        /*
            STATES: Idle, Ready, Waiting, Running, Result
            ACTIONS: Tick(), GoStopPressed(), CoinInserted()

            I am using nested classes in this class to have the Abstract State class and all the concrete State clases.
        */
        // Shared constants (visible to all nested classes)
        protected const int MAX_REACTION_TICKS = 200;  // 2.0 seconds (200 * 10ms)
        protected const int MAX_TRIALS = 3;
        protected const int MAX_READY_TICKS = 1000;

        private IGui? _gui;
        private IRandom? _randomNumber;
        private State? _currentState;

        // For averaging reaction time - requires running total and average
        protected int averageReactionTicks = 0;
        protected int sum_reaction_times = 0;
        protected int trialCount = 0;

        // INTERFACE METHODS 
        // Initialise the Controller to the Idle State
        public void Init(){
            _currentState = new IdleState(this);
        }

        public void Connect(IGui gui, IRandom rng) 
        {
            _gui = gui;
            _randomNumber = rng;
            Init();
        }


        // Call state handleRS
        public void Tick() => _currentState?.Tick();
        public void GoStopPressed() => _currentState?.GoStopPressed();
        public void CoinInserted() => _currentState?.CoinInserted();

        
        // base class State for all States - Private so that they can be accessible only within the controller(parent class of nested)
        private abstract class State {
            // _controller set to protected so that it is accessible only to the base classes and subclasses. External class such as SimpleReactionMachine cannot access it - Safer
            protected EnhancedReactionController _controller;
            
            // Constructor creates a State and make it a State of the Simple Reaction Controller
            public State(EnhancedReactionController controller){
                _controller = controller;
            }

            // Abstract methods will be implemented in the concrete state classes - This enable for Dynamic State Dispatch. Method of derived class in only determined at runtime by actual instance type of the identifier
            public abstract void CoinInserted();
            public abstract void GoStopPressed();
            public abstract void Tick();

        }

        // Concrete state classes
        private class IdleState: State{
            // CoinInserted() -> ReadyState

            // Contructor calls base contructor with the controller parameter
            public IdleState(EnhancedReactionController controller): base(controller){
                // Initialize initial display behaviour
                _controller._gui?.SetDisplay("Insert Coin");
                // initialize trialCount to 0
                _controller.trialCount = 0;
                _controller.sum_reaction_times = 0;
                _controller.averageReactionTicks = 0; // Reset average as well
            }

            public override void CoinInserted()
            {
                // Switch to ReadyState
                _controller._currentState = new ReadyState(_controller);
            }

            public override void GoStopPressed()
            {
                // Do nothing
            }

            public override void Tick() {
                
            } // DO NOTHING

        }

        private class ReadyState: State{
            private int elapsedTicks;
            // Contructor calls base contructor with the controller parameter
            public ReadyState(EnhancedReactionController controller): base(controller){
                // Initialize initial display behaviour
                _controller._gui?.SetDisplay("Press GO!");
                // Resets trialCount
                controller.trialCount++;
                // resets elapsedTicks
                elapsedTicks = 0;
            }

            public override void CoinInserted()
            {
                // Do nothing
            }

            public override void GoStopPressed()
            {
                _controller._currentState = new WaitingState(_controller);
            }

            public override void Tick() {
                // if GoStop not pressed withing 10 seconds, abort game
                elapsedTicks++;
                if (elapsedTicks >= MAX_READY_TICKS){
                    _controller._currentState = new IdleState(_controller);
                }
            } 

        }

        private class WaitingState: State{
            // GoStopPressed() -> Idle (Cheating)
            // Waits for randomTime and then -> ReadyState
            
            // Ticks for the Timer mechanism
            private int _elapsedTicks;
            private readonly int _delayTicks;

            // Contructor calls base contructor with the controller parameter
            public WaitingState(EnhancedReactionController controller): base(controller){
                // Initialize initial display behaviour
                _controller._gui?.SetDisplay("Trial: " +_controller.trialCount + "    Wait..." );
                _delayTicks = _controller._randomNumber?.GetRandom(100,250) ?? 150; // 1 tick every 10ms. Random time between 1s and 2.5s.
                // / Default to 150 if randomNumber is null 
            }

            public override void CoinInserted()
            {
                // Do nothing
            }

             public override void GoStopPressed()
            {
                _controller._currentState = new IdleState(_controller);
            }

            public override void Tick()
            {   
                // Controller.Tick() is called every 10 ms, so we must increment our elapsedTicks each time its called
                
                _elapsedTicks++;
        
                if (_elapsedTicks >= _delayTicks){
                    // switch to Running State
                    _controller._currentState = new RunningState(_controller);
                }
                
            }

        }

        private class RunningState: State{
            // Starts _reactionTicks 
            // After _reactionTicks = 200, Switch to ResultState
            // GoStopPressed() -> ResultState
            private int _reactionTicks;
            private decimal decimalTicks;
            public RunningState(EnhancedReactionController controller): base(controller){
                // Initialize initial display behaviour
                _reactionTicks = 0;
                _controller._gui?.SetDisplay((_reactionTicks / 100.0).ToString("0.00"));
            }

            public override void CoinInserted()
            {
                // Do nothing
            }

            public override void GoStopPressed()
            {
                // if 3 trials done, calculate average
                _controller.sum_reaction_times = _controller.sum_reaction_times + _reactionTicks;
                if(_controller.trialCount == 3){
                    decimalTicks = Convert.ToDecimal(_controller.sum_reaction_times)/MAX_TRIALS;
                    // Correct mathematical rounding for average ticks
                    _controller.averageReactionTicks = (int)Math.Round(decimalTicks, MidpointRounding.AwayFromZero);
                }
                // Switch to ResultState
                _controller._currentState = new ResultState(_controller,_reactionTicks);

            }

            public override void Tick()
            {
                _reactionTicks++;
                // Update display every tick (formatted as 0.00)
                _controller._gui?.SetDisplay((_reactionTicks / 100.0).ToString("0.00"));
                if (_reactionTicks >= MAX_REACTION_TICKS){
                    // Switch to result screen even if user is too slow (>2s)
                    // if 3 trials done, calculate average
                    _controller.sum_reaction_times = _controller.sum_reaction_times + _reactionTicks;
                    if(_controller.trialCount == 3){
                        _controller.averageReactionTicks = _controller.sum_reaction_times/MAX_TRIALS;
                    }
                    // Switch to ResultState
                    _controller._currentState = new ResultState(_controller,_reactionTicks);
                }
            }
        }

        private class ResultState: State{
            private int _elapsedTicks;
            private int _measuredReactionTicks;
            private const int DELAYTICKS = 300; // 3s for delay before switching

            public ResultState(EnhancedReactionController controller, int measuredReactionTicks): base(controller){
                _elapsedTicks = 0;
                _measuredReactionTicks = measuredReactionTicks;
                _controller._gui?.SetDisplay("Reaction time: " + (_measuredReactionTicks/100.0).ToString("0.00"));
            }

            public override void CoinInserted()
            {
                // Do nothing
            }

            public override void GoStopPressed()
            {
                // transitions to FinalResult state if 3 trials reached
                if (_controller.trialCount == MAX_TRIALS)
                {
                    _controller._currentState = new FinalResultState(_controller);
                }
                else
                {
                    _controller._currentState = new ReadyState(_controller);
                }
            }

            public override void Tick()
            {   
                _elapsedTicks++;
                if (_elapsedTicks >= DELAYTICKS){
                    if(_controller.trialCount == MAX_TRIALS){
                        _controller._currentState = new FinalResultState(_controller);
                    }else{
                        _controller._currentState = new ReadyState(_controller);
                    }
                }
            }
        }

        private class FinalResultState: State{
            private int _elapsedTicks;
            private const int DELAYTICKS = 500; // 5s delay for average display

            public FinalResultState(EnhancedReactionController controller): base(controller){
                _elapsedTicks = 0;
                _controller._gui?.SetDisplay("Avg time: " + (_controller.averageReactionTicks/100.0).ToString("0.00"));
            }

            public override void CoinInserted()
            {
                // Do nothing
            }

            public override void GoStopPressed()
            {
                _controller._currentState = new IdleState(_controller);
            }

            public override void Tick()
            {   
                _elapsedTicks++;
                // transitions to IdleState after 5s delay
                if (_elapsedTicks >= DELAYTICKS)
                {
                    _controller._currentState = new IdleState(_controller);
                }
            }
        }
    } 
}
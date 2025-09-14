using System;

namespace SimpleReactionMachine
{
    class Tester
    {
        static private IController controller = null!;
        static private IGui gui = null!;
        private static string displayText;
        private static int randomNumber;
        private static int passed = 0;

        static void Main(string[] args)
        {
            // run simple test
            SimpleTest();
            Console.WriteLine("\n=====================================\nSummary: {0} tests passed out of 58", passed);
            Console.ReadKey();
        }

        private static void SimpleTest()
        {
            //Construct a ReactionController
            controller = new EnhancedReactionController();
            gui = new DummyGui();

            //Connect them to each other
            gui.Connect(controller);
            controller.Connect(gui, new RndGenerator());

            //Reset the components()
            gui.Init();

            // Test the EnhancedReactionController with 5 new scenarios
            /**
                Test scenarios: 
                1. Single Trial, check measured reaction time
                2. 3 Trials and check average reaction time
                3. Check correct transition when GoStop pressed while waiting (cheating scenario) 
                4. Check correct transition after 10 seconds ready ticks before going back to idle
                5. Check correct transition when GoStop pressed while showing result
            */

            // Scenario 1 - Single Trial

            //IDLE
            DoReset(1, controller, "Insert coin");
            DoGoStop(2, controller, "Insert coin");
            DoTicks(3, controller, 1, "Insert coin");

            //coinInserted
            DoInsertCoin(4, controller, "Press GO!");

            //READY
            DoTicks(5, controller, 1, "Press GO!");
            DoInsertCoin(6, controller, "Press GO!");

            //goStop
            randomNumber = 117;
            DoGoStop(7, controller, "Trial: 1    Wait...");

            //WAIT tick(s)
            DoTicks(8, controller, randomNumber - 1, "Trial: 1    Wait...");

            //RUN tick(s)
            DoTicks(9, controller, 1, "0.00");
            DoTicks(10, controller, 1, "0.01");
            DoTicks(11, controller, 11, "0.12");
            DoTicks(12, controller, 111, "1.23");

            //goStop
            DoGoStop(13, controller, "Reaction time: 1.23");

            //STOP tick(s) -  check waiting 3 seconds for on results screen
            DoTicks(14, controller, 299, "Reaction time: 1.23");

            // Scenario 2 - Multiple trials and average time 
            DoReset(15, controller, "Insert coin");

            // 1st trial
            DoInsertCoin(16, controller, "Press GO!");
            randomNumber = 100;
            DoGoStop(17, controller, "Trial: 1    Wait...");
            DoTicks(18, controller, randomNumber, "0.00");
            DoTicks(19, controller, 50, "0.50");
            DoGoStop(20, controller, "Reaction time: 0.50");
            DoTicks(21, controller, 300, "Press GO!");

            // 2nd trial
            DoGoStop(22, controller, "Trial: 2    Wait...");
            randomNumber = 100;
            DoTicks(23, controller, randomNumber, "0.00");
            DoTicks(24, controller, 75, "0.75");
            DoGoStop(25, controller, "Reaction time: 0.75");
            DoTicks(26, controller, 300, "Press GO!");

            // 3rd trial
            DoGoStop(27, controller, "Trial: 3    Wait...");
            randomNumber = 100;
            DoTicks(28, controller, randomNumber, "0.00");
            DoTicks(29, controller, 60, "0.60");
            DoGoStop(30, controller, "Reaction time: 0.60");
            DoTicks(31, controller, 300, "Avg time: 0.62"); // Now shows only average
            DoTicks(32, controller, 500, "Insert coin"); // After 5s, back to idle

            // Scenario 3 - Cheating detection 
            DoReset(33, controller, "Insert coin");
            DoInsertCoin(34, controller, "Press GO!");
            randomNumber = 150;
            DoGoStop(35, controller, "Trial: 1    Wait...");
            DoTicks(36, controller, randomNumber/2, "Trial: 1    Wait...");
            DoGoStop(37, controller, "Insert coin"); // Cheating detected

            // Scenario 4 - Max ready wait is 10 seconds
            DoReset(38, controller, "Insert coin");
            DoInsertCoin(39, controller, "Press GO!");
            DoTicks(40, controller, 1000, "Insert Coin");

            // Scenario 5 - Transition when GoStop pressed in result state
            DoReset(41, controller, "Insert coin");

            // 1st trial
            DoInsertCoin(42, controller, "Press GO!");
            randomNumber = 100;
            DoGoStop(43, controller, "Trial: 1    Wait...");
            DoTicks(44, controller, randomNumber, "0.00");
            DoTicks(45, controller, 50, "0.50");
            DoGoStop(46, controller, "Reaction time: 0.50");
            // GoStop should transition to next trial - check 1 of scenario 5
            DoGoStop(47, controller, "Press GO!"); 

            // 2nd trial
            DoGoStop(48, controller, "Trial: 2    Wait...");
            randomNumber = 100;
            DoTicks(49, controller, randomNumber, "0.00");
            DoTicks(50, controller, 75, "0.75");
            DoGoStop(51, controller, "Reaction time: 0.75");
            DoTicks(52, controller, 300, "Press GO!"); // Wait for next trial

            // 3rd trial
            DoGoStop(53, controller, "Trial: 3    Wait...");
            randomNumber = 100;
            DoTicks(54, controller, randomNumber, "0.00");
            DoTicks(55, controller, 60, "0.60");
            DoGoStop(56, controller, "Reaction time: 0.60");
            // GoStop should transition to FinalResultState
            DoGoStop(57, controller, "Avg time: 0.62"); 
            // GoStop in FinalResultState transitions to Idle
            DoGoStop(58, controller, "Insert coin");    



        


        }

        private static void DoReset(int testNo, IController controller, string msg)
        {
            try
            {
                controller.Init();
                GetMessage(testNo, msg);
            }
            catch (Exception exception)
            {
                Console.WriteLine("test {0}: failed with exception {1})", testNo, msg, exception.Message);
            }
        }

        private static void DoGoStop(int testNo, IController controller, string msg)
        {
            try
            {
                controller.GoStopPressed();
                GetMessage(testNo, msg);
            }
            catch (Exception exception)
            {
                Console.WriteLine("test {0}: failed with exception {1})", testNo, msg, exception.Message);
            }
        }

        private static void DoInsertCoin(int testNo, IController controller, string msg)
        {
            try
            {
                controller.CoinInserted();
                GetMessage(testNo, msg);
            }
            catch (Exception exception)
            {
                Console.WriteLine("test {0}: failed with exception {1})", testNo, msg, exception.Message);
            }
        }

        private static void DoTicks(int ch, IController controller, int n, string msg)
        {
            try
            {
                for (int t = 0; t < n; t++) controller.Tick();
                GetMessage(ch, msg);
            }
            catch (Exception exception)
            {
                Console.WriteLine("test {0}: failed with exception {1})", ch, msg, exception.Message);
            }
        }

        private static void GetMessage(int ch, string msg)
        {
            if (msg.ToLower() == displayText.ToLower())
            {
                Console.WriteLine("test {0}: passed successfully", ch);
                passed++;
            }
            else
                Console.WriteLine("test {0}: failed with message ( expected {1} | received {2})", ch, msg, displayText);
        }

        private class DummyGui : IGui
        {

            private IController controller;

            public void Connect(IController controller)
            {
                this.controller = controller;
            }

            public void Init()
            {
                displayText = "?reset?";
            }

            public void SetDisplay(string msg)
            {
                displayText = msg;
            }
        }

        private class RndGenerator : IRandom
        {
            public int GetRandom(int from, int to)
            {
                return randomNumber;
            }
        }

    }

}

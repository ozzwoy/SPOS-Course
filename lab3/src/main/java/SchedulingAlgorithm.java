// Run() is called from Scheduling.main() and is where
// the scheduling algorithm written by the user resides.
// User modification should occur within the Run() function.

import java.util.Vector;
import java.io.*;

public class SchedulingAlgorithm {
  /*
  * The method runs process scheduling simulation with guaranteed scheduling
  * */
  public static Results run(int runtime, int quantum, Vector processVector, Results result, String resultsFile) {
    //total time passed (a number of simulation cycle iterations)
    int comptime = 0;
    //actual time
    long start;
    int currentProcess = 0;
    int previousProcess;
    int size = processVector.size();
    //total processes completed
    int completed = 0;
    //ratio of total cpu time passed for the current process to a planned time, initial ratio is 0/0
    String currentRatio = "0/0";
    boolean needSwitch = false;
    boolean isPreempted = false;

    result.schedulingType = "Interactive (Preemptive)";
    result.schedulingName = "Guaranteed";
    try {
      //BufferedWriter out = new BufferedWriter(new FileWriter(resultsFile));
      //OutputStream out = new FileOutputStream(resultsFile);
      PrintStream out = new PrintStream(new FileOutputStream(resultsFile));
      sProcess process = (sProcess) processVector.elementAt(currentProcess);
      start = System.currentTimeMillis();
      out.println("Time elapsed: " + (System.currentTimeMillis() - start) + "ms   Process: " + currentProcess +
                  " registered... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " +
                  currentRatio + ")");

      while (comptime < runtime) {
        //is the process finished (stated working time is reached)
        if (process.cpudone == process.cputime) {
          completed++;
          currentRatio = String.valueOf(process.cpudone) + "/(" + String.valueOf(comptime) + "/" +
                         String.valueOf(size) + ")";
          out.println("Time elapsed: " + (System.currentTimeMillis() - start) + "ms\tProcess: " + currentProcess +
                      " completed... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " +
                      currentRatio + ")");
          if (completed == size) {
            result.compuTime = comptime;
            out.close();
            return result;
          }
          needSwitch = true;
        }
        //if the process is blocked for I/O (stated time before I/O has passed)
        else if (process.ioblocking == process.ionext) {
          currentRatio = String.valueOf(process.cpudone) + "/(" + String.valueOf(comptime) + "/" +
                  String.valueOf(size) + ")";
          out.println("Time elapsed: " + (System.currentTimeMillis() - start) + "ms\tProcess: " + currentProcess +
                      " I/O blocked... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " +
                      currentRatio + ")");
          process.numblocked++;
          process.ionext = 0;
          process.preemptednext = 0;
          needSwitch = true;
        }
        //if the process has spent its quantum
        else if (process.preemptednext == quantum) {
          currentRatio = String.valueOf(process.cpudone) + "/(" + String.valueOf(comptime) + "/" +
                  String.valueOf(size) + ")";
          out.println("Time elapsed: " + (System.currentTimeMillis() - start) + "ms\tProcess: " + currentProcess +
                      " preempted... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " +
                      currentRatio + ")");
          process.numpreempted++;
          process.preemptednext = 0;
          needSwitch = true;
          isPreempted = true;
        }
        //switching to other process by the rules of guaranteed scheduling
        if (needSwitch) {
          double ratio;
          double minRatio = Double.MAX_VALUE;
          previousProcess = currentProcess;
          //looking for process with a minimal ratio of total cpu time passed for the current process to a planned time
          for (int i = 0; i < size; i++) {
            process = (sProcess) processVector.elementAt(i);
            ratio = (double) process.cpudone / ((double) comptime / size);
            //if the process was preempted, then it can compete for the next quantum, otherwise not
            if (process.cpudone < process.cputime && ratio < minRatio && (i != previousProcess || isPreempted)) {
              minRatio = ratio;
              currentProcess = i;
            }
          }

          process = (sProcess) processVector.elementAt(currentProcess);
          currentRatio = String.valueOf(process.cpudone) + "/(" + String.valueOf(comptime) + "/" +
                         String.valueOf(size) + ")";
          out.println("Time elapsed: " + (System.currentTimeMillis() - start) + "ms\tProcess: " + currentProcess +
                      " registered... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " +
                      currentRatio + ")");
          needSwitch = false;
          isPreempted = false;
        }

        //increment the process' spent CPU time
        process.cpudone++;
        //increment time passed after getting quantum
        process.preemptednext++;
        if (process.ioblocking > 0) {
          //increment time passed after last I/O
          process.ionext++;
        }
        //increment total CPU time passed
        comptime++;
      }
      out.close();
    } catch (IOException e) { /* Handle exceptions */ }

    result.compuTime = comptime;
    return result;
  }
}

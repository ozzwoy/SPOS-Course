// Run() is called from Scheduling.main() and is where
// the scheduling algorithm written by the user resides.
// User modification should occur within the Run() function.

import java.util.Vector;
import java.io.*;

public class SchedulingAlgorithm {

  public static Results Run(int runtime, int quantum, Vector processVector, Results result) {
    int comptime = 0;
    int currentProcess = 0;
    int previousProcess;
    int size = processVector.size();
    int completed = 0;
    String currentRatio = "0/0";
    boolean needSwitch = false;
    boolean isPreempted = false;
    String resultsFile = "src/main/resources/Summary-Processes";

    result.schedulingType = "Interactive (Preemptive)";
    result.schedulingName = "Guaranteed";
    try {
      //BufferedWriter out = new BufferedWriter(new FileWriter(resultsFile));
      //OutputStream out = new FileOutputStream(resultsFile);
      PrintStream out = new PrintStream(new FileOutputStream(resultsFile));
      sProcess process = (sProcess) processVector.elementAt(currentProcess);
      out.println("Process: " + currentProcess + " registered... (" + process.cputime + " " + process.ioblocking + " " +
                  process.cpudone + " " + currentRatio + ")");

      while (comptime < runtime) {
        if (process.cpudone == process.cputime) {
          completed++;
          currentRatio = String.valueOf(process.cpudone) + "/(" + String.valueOf(comptime) + "/" +
                         String.valueOf(size) + ")";
          out.println("Process: " + currentProcess + " completed... (" + process.cputime + " " + process.ioblocking +
                      " " + process.cpudone + " " + currentRatio + ")");
          if (completed == size) {
            result.compuTime = comptime;
            out.close();
            return result;
          }
          needSwitch = true;
        } else if (process.ioblocking == process.ionext) {
          currentRatio = String.valueOf(process.cpudone) + "/(" + String.valueOf(comptime) + "/" +
                  String.valueOf(size) + ")";
          out.println("Process: " + currentProcess + " I/O blocked... (" + process.cputime + " " + process.ioblocking +
                  " " + process.cpudone + " " + currentRatio + ")");
          process.numblocked++;
          process.ionext = 0;
          process.preemptednext = 0;
          needSwitch = true;
        } else if (process.preemptednext == quantum) {
          currentRatio = String.valueOf(process.cpudone) + "/(" + String.valueOf(comptime) + "/" +
                  String.valueOf(size) + ")";
          out.println("Process: " + currentProcess + " preempted... (" + process.cputime + " " + process.ioblocking +
                      " " + process.cpudone + " " + currentRatio + ")");
          process.numpreempted++;
          process.preemptednext = 0;
          needSwitch = true;
          isPreempted = true;
        }

        if (needSwitch) {
          double ratio;
          double minRatio = Double.MAX_VALUE;
          previousProcess = currentProcess;
          for (int i = 0; i < size; i++) {
            process = (sProcess) processVector.elementAt(i);
            ratio = (double) process.cpudone / ((double) comptime / size);
            if (process.cpudone < process.cputime && ratio < minRatio && (i != previousProcess || isPreempted)) {
              minRatio = ratio;
              currentProcess = i;
            }
          }
          process = (sProcess) processVector.elementAt(currentProcess);
          currentRatio = String.valueOf(process.cpudone) + "/(" + String.valueOf(comptime) + "/" +
                  String.valueOf(size) + ")";
          out.println("Process: " + currentProcess + " registered... (" + process.cputime + " " + process.ioblocking +
                      " " + process.cpudone + " " + currentRatio + ")");
          needSwitch = false;
          isPreempted = false;
        }

        process.cpudone++;
        process.preemptednext++;
        if (process.ioblocking > 0) {
          process.ionext++;
        }
        comptime++;
      }
      out.close();
    } catch (IOException e) { /* Handle exceptions */ }

    result.compuTime = comptime;
    return result;
  }
}

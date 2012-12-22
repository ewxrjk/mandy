using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace uk.org.greenend.mandy
{
  /// <summary>
  /// A job in the job queue
  /// </summary>
  public class Job
  {
    /// <summary>
    /// Context object (for cancellation)
    /// </summary>
    internal object context;

    /// <summary>
    /// Called in a background thread to run the job.
    /// </summary>
    /// <remarks>The default implementation does nothing.</remarks>
    public virtual void Run()
    {
    }

    /// <summary>
    /// Called from JobQueue.Complete to complete the job.
    /// </summary>
    /// <remarks>The default implementation does nothing.</remarks>
    public virtual void Complete()
    {
    }

    /// <summary>
    /// Called if a job is cancelled without being run
    /// </summary>
    /// <remarks>The default implementation does nothing.</remarks>
    public virtual void Cancel()
    {
    }
  }

  /// <summary>
  /// The job queue
  /// </summary>
  public static class JobQueue
  {
    /// <summary>
    /// Target number of threads
    /// </summary>
    /// <remarks>
    /// <para>Don't change this after adding any jobs - it will be ignored.</para>
    /// <para>The default is the number of logical CPUs.</para>
    /// </remarks>
    public static int Workers = Environment.ProcessorCount;

    /// <summary>
    /// Set to true when worker threads have been created.
    /// </summary>
    private static bool workersCreated = false;

    /// <summary>
    /// Queued jobs
    /// </summary>
    private static LinkedList<Job> jobsPending = new LinkedList<Job>();

    /// <summary>
    /// Jobs currently being processed
    /// </summary>
    private static HashSet<Job> jobsWorking = new HashSet<Job>();

    /// <summary>
    /// Completed jobs awaiting collection
    /// </summary>
    private static LinkedList<Job> jobsComplete = new LinkedList<Job>();

    /// <summary>
    /// Worker thread
    /// </summary>
    static private void Worker()
    {
      Monitor.Enter(jobsPending);
      while (true)
      {
        if (jobsPending.Count > 0)
        {
          // There is work to be done
          Job j = jobsPending.First.Value;
          jobsPending.RemoveFirst();
          jobsWorking.Add(j);
          Monitor.Exit(jobsPending);
          try
          {
            j.Run();
          }
          catch (Exception)
          {
            // Jobs that throw exceptions just lose them.
          }
          lock (jobsComplete)
          {
            jobsComplete.AddLast(j);
            Monitor.PulseAll(jobsComplete);
          }
          // NB that a job can be in both jobsWorking and jobsComplete
          // at the same time.
          Monitor.Enter(jobsPending);
          jobsWorking.Remove(j);
          continue;
        }
        // If we didn't find anything to do, wait
        Monitor.Wait(jobsPending);
      }
      Monitor.Exit(jobsPending);
    }

    /// <summary>
    /// Create a new worker thread
    /// </summary>
    /// <remarks><para>Called with lock held.</para></remarks>
    static private void CreateWorkers()
    {
      for (int n = 0; n < Workers; ++n)
      {
        Thread newThread = new Thread(new ThreadStart(Worker))
        {
          IsBackground = true
        };
        newThread.Start();
      }
      workersCreated = true;
    }
    
    /// <summary>
    /// Add a job to the queue
    /// </summary>
    /// <param name="job">Job to enqueue</param>
    /// <param name="context">Context information</param>
    public static void Add(Job job, object context)
    {
      lock (jobsPending)
      {
        if (!workersCreated)
        {
          CreateWorkers();
        }
        job.context = context;
        jobsPending.AddLast(job);
        Monitor.Pulse(jobsPending);
      }
    }

    /// <summary>
    /// Cancel all pending jobs for a given context
    /// </summary>
    /// <param name="context">Context to remove</param>
    /// <remarks><para>Note that working and completed jobs are not removed.</para></remarks>
    // TODO but maybe they should be - we could promise that once
    // Cancel() has been called you'll never see anything from that
    // context again, so it'd be a guarantee rather than an optimization.
    public static void Cancel(object context)
    {
      List<Job> cancelled = new List<Job>();
      lock (jobsPending)
      {
        for (var node = jobsPending.First; node != null; )
        {
          var next = node.Next;
          if (node.Value.context == context)
          {
            jobsPending.Remove(node);
            // The cancel callback had better be issued outside the lock
            cancelled.Add(node.Value);
          }
          node = next;
        }
      }
      foreach (var job in cancelled)
      {
        job.Cancel();
      }
    }

    /// <summary>
    /// Run completion for one or more completed jobs
    /// </summary>
    /// <param name="count">Maximum number of job completions to run</param>
    /// <param name="context">If not null, only complete jobs in this context</param>
    /// <param name="block">If true, block until all jobs completed, or queue empty</param>
    /// <returns>The number of jobs that were completed.</returns>
    /// <remarks><para>The intended use is for Complete to be restricted
    /// to a single thread.  However, it should be safe to call it from multiple
    /// threads, provided you don't mind your Complete() overrides running
    /// in unpredictable threads (if context isn't set).</para></remarks>
    public static int Complete(int count, object context, bool block)
    {
      // Special case a count of 0 - it's a stupid value to pass
      // but it shouldn't cause a hang.
      if (count == 0)
      {
        return 0;
      }
      int completed = 0;
      Monitor.Enter(jobsComplete);
      while (true)
      {
        int completedThisTime = 0;
        for (var node = jobsComplete.First; count > 0 && node != null; )
        {
          var next = node.Next;
          if (context == null || node.Value.context == context)
          {
            // Found a suitable job.  Remove it from the list
            // and then call Complete() on it.
            jobsComplete.Remove(node);
            Monitor.Exit(jobsComplete);
            node.Value.Complete();
            ++completedThisTime;
            --count;
            Monitor.Enter(jobsComplete);
          }
          node = next;
        }
        completed += completedThisTime;
        if (count == 0)
        {
          // We've done as much work as we were asked to do.
          break;
        }
        if (completedThisTime > 0)
        {
          // If we did any work we'll have released the lock, so it's
          // possible that more work arrived in the meantime.  Go back
          // and check.
          continue;
        }
        if (!block)
        {
          // We didn't find any work to do.  If we were asked not to block,
          // exit now.
          break;
        }
        // Wait for more work to arrive.
        Monitor.Wait(jobsComplete);
      }
      Monitor.Exit(jobsComplete);
      return completed;
    }
  }
}

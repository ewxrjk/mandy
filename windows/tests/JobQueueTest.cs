using System;
using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using uk.org.greenend.mandy;

namespace tests
{
  /// <summary>
  /// Test job that just records what happened to it
  /// </summary>
  class TestJob : Job
  {
    public override void Run()
    {
      hasRun = true;
    }

    public override void Complete()
    {
      hasCompleted = true;
    }

    public override void Cancel()
    {
      hasCancelled = true;
    }

    public bool hasRun = false;

    public bool hasCompleted = false;

    public bool hasCancelled = false;
  }

  /// <summary>
  /// Tests for JobQueue
  /// </summary>
  [TestClass]
  public class JobQueueTest
  {
    /// <summary>
    /// Test that jobs are run and completed
    /// </summary>
    [TestMethod]
    public void RunJobs()
    {
      const int nJobs = 128;
      List<TestJob> jobs = new List<TestJob>();
      // Create a collection of jobs and run them
      for (int i = 0; i < nJobs; ++i)
      {
        TestJob job = new TestJob();
        jobs.Add(job);
        JobQueue.Add(job, this);
      }
      int complete = JobQueue.Complete(nJobs, null, true);
      Assert.AreEqual(nJobs, complete, string.Format("only {0} jobs completed", complete));
      for(int i = 0; i < nJobs; ++i)
      {
        var job = jobs[i];
        Assert.AreEqual(true, job.hasRun, string.Format("job {0} wasn't run", i));
        Assert.AreEqual(true, job.hasCompleted, string.Format("job {0} wasn't completed", i));
      }
    }

    /// <summary>
    /// Test that cancellation works
    /// </summary>
    [TestMethod]
    public void CancelJobs()
    {
      const int nJobs = 128;
      List<TestJob> classOneJobs = new List<TestJob>();
      List<TestJob> classTwoJobs = new List<TestJob>();
      for (int i = 0; i < nJobs; ++i)
      {
        TestJob job = new TestJob();
        classOneJobs.Add(job);
        JobQueue.Add(job, classOneJobs);
        job = new TestJob();
        classTwoJobs.Add(job);
        JobQueue.Add(job, classTwoJobs);
      }
      JobQueue.Cancel(classTwoJobs);
      int complete = JobQueue.Complete(2 * nJobs, null, true);
      Assert.IsTrue(complete >= nJobs, string.Format("only {0} jobs completed", complete));
      for (int i = 0; i < nJobs; ++i)
      {
        var job = classOneJobs[i];
        Assert.AreEqual(true, job.hasRun, string.Format("class one job {0} wasn't run", i));
        Assert.AreEqual(true, job.hasCompleted, string.Format("class one job {0} wasn't completed", i));
        job = classTwoJobs[i];
        if (job.hasRun)
        {
          Assert.AreEqual(true, job.hasCompleted, string.Format("class two job {0} run but wasn't completed", i));
        }
        else
        {
          Assert.AreEqual(false, job.hasCompleted, string.Format("class two job {0} didn't run but was completed", i));
          Assert.AreEqual(true, job.hasCancelled, string.Format("class two job {0} didn't run but wasn't cancelled", i));
        }
      }  
    }
  }
}

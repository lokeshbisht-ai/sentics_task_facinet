# sentics_task_facinet

<p> Here is my solution: <p>
  
  <li> I Read the dumy_data and store in container based on columns</li>
  
  <li>I Sort by timestamp but not necessary here because it's already sorted by timestamp</li>
  
  <li> I group by sensorid </li>
  
  <li>I For each sensor group, group elements by uniq_id</li>
  
  <li>I For each sensor uniq_id groups, group by distance </li>
  
  <li> I merge by distance groups </li>
 
  <li> I  creat  a cluster and to push it to my list of clusters </li> <br/>
  
  <p> I tried to balance beetwen quality and time, if given chance I could optimized and refactor some part of the code and make it more efficient <p/>
  <p> I'm looking forward to hear your feedback </li>

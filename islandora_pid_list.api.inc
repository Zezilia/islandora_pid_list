<?php
class Islandora_Pid_List_Helper {
  
  public function get_user_pid_list($uid) {
    // SESSION CHECKING THINGER IF SESSION JUST GRAB FROM SESSION TABLE?
    module_load_include('inc', 'islandora_pid_list', 'pidlist');
    
    // If user is not an anon
    if ($uid != 0) {
      $listids = array();
      $pidlist = array();
    
      $result = db_query('SELECT listid FROM {islandora_pid_list_lists} WHERE uid = %d', $uid);
    
      while ($listid = db_fetch_array($result)) {
        array_push($listids, $listid['listid']);
      }
   
      foreach ($listids as $key => $value) {
        $listpids = array();
        $listusers = array();
    
      $result = db_query('SELECT pidid from {islandora_pid_list_pids} WHERE listid = %d', $value);
    
      while ($pid = db_fetch_array($result)) {
        array_push($listpids, $pid['pidid']);
      }
            
      $listuser = db_query('SELECT uid from {islandora_pid_list_lists} WHERE listid = %d', $value);
      
      while ($user = db_fetch_array($listuser)) {
        array_push($listusers, $user['uid']);
      }
    
      $listresult = db_query('SELECT listname, listowner from {islandora_pid_list_names} WHERE listid = %d', $value);
      $listresult = db_fetch_array($listresult);
        
      $listid = $value;
    
      $params = array(
        "retrieve" => TRUE,
        "useruid" => $uid,
        "listowner" => $listresult['listowner'],
        "listusers" => $listusers,
        "listpids" => $listpids,
        "listname" => $listresult['listname'],
        "listid" => $listid
      );
      $templist = new PidList($params);
       
      $pidlist[] = $templist;
      }
      return $pidlist;
    }
    // User is anon, use session
    else {
      $pidlist = array();
        if (!empty($_SESSION['islandora_pid_list'])) {
          foreach($_SESSION['islandora_pid_list'] as $value) {
            $pidlist[] = unserialize($value);
          }
        }
    return $pidlist;
    }
  }
 }
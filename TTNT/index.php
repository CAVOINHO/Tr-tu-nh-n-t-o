<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <link rel="stylesheet" href="./style.css">
  <title>Tìm đường đi trong mê cung</title>
</head>
<body onload="thewaytogoal()">
<?php
    $file = @fopen('maze.txt', 'r');
    $i=0;
    $index=1;
    echo "<table border='0'>";
      echo "<tr>";
    while (!feof($file)) {
        $a=fgetc($file);
        if($a=="0" || $a=="1" || $a=="2" || $a=="3"){
          if($a=="1")
            echo "<td id='".$index."' class='wall'></td>";
          else if($a=="2")
            echo "<td id='".$index."' class='start'></td>";
          else if($a=="3")
            echo "<td id='".$index."' class='goal'></td>";
          else 
            echo "<td id='".$index."'></td>";
          $i++;
          $index++;
          if($i==25){
            echo "</tr>";
            echo "<tr>";
            $i=0;
          }
        }
    }
    echo "</tr>";
    echo "</table>";
    echo "<div id='person'>
      <img src='./pikachu.gif' class='img-person'/>
    </div>";
    $file1 = @fopen('ketqua.txt', 'r');
    while (!feof($file1)) {
        echo "<option class='ketqua'>".fgets($file1)."</option>";
    }
?>
</body>
<script>
  var arr=document.getElementsByClassName("ketqua");
  var old_row=arr[0].value;
  var old_col=arr[1].value;
  var t =60;
  var l = 362;
  var i = 2;
  var person=document.getElementById("person");
  var show;
    function thewaytogoal(){
        if(i==arr.length)
          clearTimeout(show);
        if(arr[i].value!=old_row){
          if((arr[i].value-old_row)>0){
            t+=30;
          }
          else{
            t-=30;
          }
        }
        else{
          if((arr[i+1].value-old_col)>0){
            l+=30;
          }
          else{
            l-=30;
          }
        }
        var a=(Number(old_col)+1)+(Number(old_row)*25);
        document.getElementById(a).style.background= "#EB0B6B";
        document.getElementById(a).style.borderRadius= "50%";
        old_row=arr[i].value;
        old_col=arr[i+1].value;
        person.style.top= t+"px";
        person.style.left= l+"px";
        i+=2;
        show=setTimeout(function(){
          thewaytogoal()}, 500 );  
    }
    if(arr.value === ''){
      alert('END GAME');
    }
    
  
</script>
</html>

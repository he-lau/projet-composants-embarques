<html>
    <head>
    <meta http-equiv="content-type" content="text/html; charset=UTF-8" />

    <script src="https://cdn.plot.ly/plotly-latest.min.js"></script>

    </head>
    <body>
    <div class="navbar"><span>Temperature 18S20</span></div>

    <script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.6.0/jquery.min.js" integrity="sha512-894YE6QWD5I59HgZOGReFYm4dnWc1Qt5NtvYSaNcOP+u1T9qYdvdihz0PPSiiqn/+/3e7Jo4EaG7TubfWGUrMQ==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>

    <div id="graph"></div>

        <script>

        function getData() {

          //return Math.random();
          var a;

          $.ajax({
            url: "temp-db.php",
            type: "POST",
            data: '{ if any }',
            async: false,
            success: function(data) {
              console.log("DATA :",data.split("||")[0]);
              console.log("DATE :",Date(data.split("||")[1]));
              //a=data.split("||")[0];
              a=data;
            },
          });

         return a;
        }

        var y = getData().split("||")[0];
        var x = Date(getData().split("||")[1]);

        var time = new Date();

        var data = [{
          x: [x],
          y: [y],
          mode: 'lines',
          line: {color: '#FF0000'}
        }]


        Plotly.plot('graph', data);

        var cnt = 0;

        var interval = setInterval(function() {

          var y = getData().split("||")[0];
          var x = Date(getData().split("||")[1]);


          var update = {
          x:  [[x]],
          y: [[y]]
          }

          Plotly.extendTraces('graph', update, [0])

          if(cnt === 100) clearInterval(interval);
        }, 500);


        </script>
    </body>
</html>

const char MAIN_page[] PROGMEM = R"=====(
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Morse Code Blinker</title>
  <script src="https://code.jquery.com/jquery-3.7.1.min.js"></script>
</head>
<body>

  <label for="inputText">Enter Text:</label>
  <input type="text" id="inputText" placeholder="Type a message">
  <button onclick="sendToESP()">Send to ESP</button>

  <script>
    function sendToESP() {
      var userInput = $('#inputText').val();
      
      $.get('/decode', { message: userInput }, function(response) {
        console.log(response); // You can handle the ESP response here
      })
      .fail(function(error) {
        console.error('Error sending data to ESP:', error);
      });
    }
  </script>

</body>
</html>

)=====";
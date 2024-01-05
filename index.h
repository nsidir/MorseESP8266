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

  <div id="morseWord"></div>

  <script>
    const morseCodeMapping = {
    '.-': 'A', '-...': 'B', '-.-.': 'C', '-..': 'D',
    '.': 'E', '..-.': 'F', '--.': 'G', '....': 'H',
    '..': 'I', '.---': 'J', '-.-': 'K', '.-..': 'L',
    '--': 'M', '-.': 'N', '---': 'O', '.--.': 'P',
    '--.-': 'Q', '.-.': 'R', '...': 'S', '-': 'T',
    '..-': 'U', '...-': 'V', '.--': 'W', '-..-': 'X',
    '-.--': 'Y', '--..': 'Z',
    '-----': '0', '.----': '1', '..---': '2', '...--': '3',
    '....-': '4', '.....': '5', '-....': '6', '--...': '7',
    '---..': '8', '----.': '9'
    };

    function morseToAlphabet(morseCode) {
      const words = morseCode.split('   '); // Morse code words are separated by three spaces
      let result = '';

      for (const word of words) {
        const letters = word.split(' ');
        for (const letter of letters) {
          result += morseCodeMapping[letter] || ''; // Use mapping, or empty string if not found
        }
        result += ' '; // Separate words with space
      }

      return result.trim(); // Remove leading/trailing spaces
    }

     $.post('/');

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
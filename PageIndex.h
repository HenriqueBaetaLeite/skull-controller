const char MAIN_page[] PROGMEM = R"=====(

<!DOCTYPE html>
<html>
  <head>
    <title>The Skull Controller</title>
    <meta name="author" content="Henrique Baêta" />
    <meta name="language" content="English" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <meta charset="UTF-8" />
    <style>
      * {
        box-sizing: border-box;
        margin: 0;
        text-align: center;
        background-color: #d9d5d7;
      }

      .slide-container {
        width: 100%;
      }

      .slider {
        -webkit-appearance: none;
        appearance: none;
      }

      input[type="range"]::-webkit-slider-runnable-track {
        background: #000000;
        border-radius: 10px;
      }

      input[type="range"]::-webkit-slider-thumb {
        -webkit-appearance: none;
        appearance: none;
        cursor: pointer;
        height: 25px;
        width: 25px;
        background-image: url("./skull.png");
        background-size: cover;
        transition: 0.2s ease-in-out;
      }

      p {
        margin: 10px;
      }

      h1,
      h4 {
        margin: 10px;
      }

      .btn {
        margin: 10px;
        border-radius: 10px;
        border: none;
        height: 25px;
        cursor: pointer;
        color: rgb(255, 255, 255);
        background-color: #8a8889;
      }

      .btn:hover {
        background-color: #555;
      }
    </style>
  </head>

  <body>
    <h1>The Skull Controller</h1>

    <img src="./skull.png" alt="skull_image" width="60px" />
    <div class="slide-container">
      <p>Mouth angle: <span id="range-value"></span></p>

      <input
        type="range"
        min="0"
        max="180"
        value="90"
        class="slider"
        id="range-slider"
      />

      <section class="button-container">
        <h4>Skull functions</h4>
        <button type="button" onclick="setSkullNumberFunction(1)" class="btn">
          Open/Close
        </button>

        <button type="button" onclick="setSkullNumberFunction(2)" class="btn">
          Open
        </button>

        <button type="button" onclick="setSkullNumberFunction(3)" class="btn">
          Half
        </button>

        <button type="button" onclick="setSkullNumberFunction(4)" class="btn">
          Close
        </button>
      </section>
    </div>

    <script>
      function sendRangeData(position) {
        const xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
          if (this.readyState == 4 && this.status == 200) {
            console.log("Resp from server: ", this.responseText);
          }
        };
        xhttp.open("GET", "setRange?servoRange=" + position, true);
        xhttp.send();
      }

      function setSkullNumberFunction(numberOfFunction) {
        const xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
          if (this.readyState == 4 && this.status == 200) {
            console.log("Resp from server: ", this.response);
          }
        };
        xhttp.open(
          "GET",
          "setServoFunction?setFunction=" + numberOfFunction,
          true
        );
        xhttp.send();
      }

      // Teste para saber se de fato vou trocar o XMLHttpRequest pelo fetch.

      function setFunctionFetch(numberOfFunction) {
        fetch("setServoFunction?setFunction=" + numberOfFunction)
          .then((response) => response.json())
          .then((data) => console.log("Here I go!", data));
      }
      setFunctionFetch(1);

      const slider = document.getElementById("range-slider");
      const rangeSpan = document.getElementById("range-value");

      rangeSpan.innerHTML = slider.value;

      slider.oninput = function () {
        rangeSpan.innerHTML = this.value;
        sendRangeData(rangeSpan.innerHTML);
      };
    </script>
  </body>
</html>

)=====";

const char MAIN_page[] PROGMEM = R"=====(

<!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <meta charset="UTF-8" />
    <style>
      * {
        box-sizing: border-box;
        margin: 0;
        text-align: center;
      }

      .slidecontainer {
        width: 100%;
      }

      .slider {
        -webkit-appearance: none;
        width: 50%;
        height: 15px;
        border-radius: 5px;
        background: rgb(102, 230, 17);
        outline: none;
        opacity: 0.5;
        -webkit-transition: 0.2s;
        transition: opacity 0.2s;
      }

      .slider:hover {
        opacity: 2;
      }

      .slider::-webkit-slider-thumb {
        -webkit-appearance: none;
        appearance: none;
        width: 25px;
        height: 25px;
        border-radius: 50%;
        background: rgb(26, 182, 202);
        cursor: pointer;
      }
      /* 
      .slider::-moz-range-thumb {
        width: 25px;
        height: 25px;
        border-radius: 50%;
        background: #4caf50;
        cursor: pointer;
      } */

      .btn {
        border-radius: 15px;
        width: 8%;
        height: 25px;
        cursor: pointer;
        background: #bdbdbd;
        color: rgb(255, 255, 255);
        background-color: #8a8889;
        color: white;
      }
    </style>
  </head>

  <body>
    <h1>The Skull Controller</h1>
    <br /><br />
    <div class="slidecontainer">
      <input
        type="range"
        min="0"
        max="180"
        value="90"
        class="slider"
        id="range-slider"
      />
      <p>ângulo da boca: <span id="range-value"></span></p>
      <br /><br />
      <label>Função abrir e fechar mandíbula</label>
      <button type="button" onclick="setSkullNumberFunction(1)" class="btn">
        Start
      </button>
      <br /><br />
      <label>Boca aberta</label>
      <button type="button" onclick="setSkullNumberFunction(2)" class="btn">
        abrir
      </button>
      <br /><br />
      <label>Boca normal</label>
      <button type="button" onclick="setSkullNumberFunction(3)" class="btn">
        normal
      </button>
      <br /><br />
      <label>Boca fechada</label>
      <button type="button" onclick="setSkullNumberFunction(4)" class="btn">
        fechar
      </button>
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
        xhttp.open("GET", "setServoFunction?setFunction=" + numberOfFunction, true);
        xhttp.send();
      }

      // Teste para saber se de fato vou trocar o XMLHttpRequest pelo fetch.

      function setFunctionFetch(numberOfFunction) {
        fetch("setServoFunction?setFunction=" + numberOfFunction)
          .then((response) => response.json())
          .then((data) => console.log("Here I go!", data));
      }
      setFunctionFetch(2);

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

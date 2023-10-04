const char MAIN_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>The Skull Controller</title>
    <meta name="author" content="Henrique Baêta" />
    <meta name="language" content="English" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <meta charset="UTF-8" />
    <link
      rel="stylesheet"
      href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css"
      integrity="sha384-rbsA2VBKQhggwzxH7pPCaAqO46MgnOM80zW1RWuH61DGLwZJEdK2Kadq2F9CUG65"
      crossorigin="anonymous"
    />
    <style>
      body {
        box-sizing: border-box;
        margin: 0;
        background-color: #a6a6a6;
        display: flex;
        justify-content: center;
        align-items: center;
        flex-direction: column;
      }

      .group-container {
        display: flex;
        justify-content: center;
        align-items: center;
        flex-direction: column;
      }

      input[type="range"] {
        -webkit-appearance: none;
      }

      input[type="range"]::-webkit-slider-runnable-track {
        background: #000000;
        border-radius: 10px;
      }

      input[type="range"]::-webkit-slider-thumb {
        -webkit-appearance: none;
        border: none;
        height: 12px;
        width: 12px;
        border-radius: 50%;
        background: goldenrod;
      }

      p,
      h1,
      h5 {
        margin-top: 20px;
      }

      .my-btn {
        margin: 7px;
        border-radius: 12px;
      }
    </style>
  </head>

  <body>
    <h1>The Skull Controller</h1>

    <!-- <img src="./skull.png" alt="skull_image" width="60px" /> -->
    <img src="https://m.media-amazon.com/images/I/61jL3Wx-xCL.__AC_SX300_SY300_QL70_FMwebp_.jpg" alt="skull_image" width="60px" />

    <div" class="group-container">
      <h5>Set mouth angle: <span id="range-value"></span></h5>

      <input
        type="range"
        min="0"
        max="180"
        value="90"
        class="slider"
        id="range-slider"
      />
    </div>

    <h5>Set skull functions</h4>
    <section class="group-container">
      <button
        type="button"
        onclick="setSkullNumberFunction(1)"
        class="btn btn-outline-dark my-btn"
      >
        Open/Close
      </button>

      <button
        type="button"
        onclick="setSkullNumberFunction(2)"
        class="btn btn-outline-dark my-btn"
      >
        Open
      </button>

      <button
        type="button"
        onclick="setSkullNumberFunction(3)"
        class="btn btn-outline-dark my-btn"
      >
        Half
      </button>

      <button
        type="button"
        onclick="setSkullNumberFunction(4)"
        class="btn btn-outline-dark my-btn"
      >
        Close
      </button>
    </section>

    <script>
      function sendRangeData(position) {
        fetch("setRange?servoRange=" + position);
      }

      function setSkullNumberFunction(numberOfFunction) {
        fetch("setServoFunction?setFunction=" + numberOfFunction);
      }

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

)rawliteral";

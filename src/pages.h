const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Configure Wifi</title>
  </head>
  <body>
    <center>
      <header>
        <h1>Configure Wifi</h1>
      </header> 
      <div>
        <form action="/save" method="POST">
          <input type="text" name="name" placeholder="AP name">
          </br>
          </br>
          <input type="password" name="password" placeholder="Password">
          </br>
          </br>
          <input type="submit" class="button" value="Save">
        </form>
      </div>
    </center>
  </body>
</html>
)=====";

const char SUCCESS_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Configure Wifi</title>
  </head>
  <body>
    <h2>New Wifi name and password are saved succesfully</h2>
    <h2>Device is restarting now with new settings</h2>
  </body>
</html>
)=====";

const char INCORRECT_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Configure Wifi</title>
  </head>
  <body>
    <h2>Please, specify Wifi name and password</h2>
    <h3>Password length must be at least 8 characters</h3>
    <button onclick="javascript:history.back()">Go Back</button>
  </body>
</html>
)=====";
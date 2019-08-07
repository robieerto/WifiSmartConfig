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
    <h1>New Wifi name and password are saved succesfully</h1>
    <h3>Device is restarting now with new settings</h3>
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
    <h1>Please, specify Wifi name and password</h1>
    <p>Password length must be at least 8 characters</p>
    <button onclick="javascript:history.back()">Go Back</button>
  </body>
</html>
)=====";

const char NOTFOUND_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Not found</title>
  </head>
  <body>
    <h1>404: Not found</h1>
    <p>The requested page was not found on this server</p>
  </body>
</html>
)=====";
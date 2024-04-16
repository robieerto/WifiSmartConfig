const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Konfigurácia Wifi</title>
  </head>
  <body>
    <div style="text-align:center">
      <header>
        <h1>Konfigurácia Wifi</h1>
      </header>
      </br>
      <div>
        <form action="/save" method="POST">
          <input type="text" name="name" style="font-size:24px;" type="text" placeholder="Názov Wifi">
          </br>
          </br>
          <input type="password" name="password" style="font-size:24px;" type="text" placeholder="Heslo">
          </br>
          </br>
          </br>
          <input type="submit" class="button" style="font-size:24px;width:120px;" value="Uložiť">
        </form>
      </div>
    </div>
  </body>
</html>
)=====";

const char SUCCESS_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Konfigurácia Wifi</title>
  </head>
  <body>
    <div style="text-align:center">
      <header>
        <h1>Nové nastavenia boli úspešne uložené</h1>
      </header>
      <p style="font-size:24px;">Zariadenie sa teraz reštartuje...</p>
    </div>
  </body>
</html>
)=====";

const char INCORRECT_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Konfigurácia Wifi</title>
  </head>
  <body>
    <div style="text-align:center">
      <header>
        <h1>Vyplňte názov a heslo vášho Wifi</h1>
      </header> 
      <p style="font-size:24px;">Dĺžka hesla býva najmenej 8 znakov</p>
      </br>
      <button onclick="javascript:history.back()" style="font-size:24px;width:100px;">Späť</button>
    </div>
  </body>
</html>
)=====";

const char NOTFOUND_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Konfigurácia Wifi</title>
  </head>
  <body>
    <h1>404: Not found</h1>
    <p>Požadovaná stránka nebola nájdená</p>
  </body>
</html>
)=====";
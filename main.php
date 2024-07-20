<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="utf8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>PHP render server</title>
        <style>
            body 
            {
                background-color: rgb(41, 41, 41);
                color: white;
            }
        </style>
    </head>
    <h1>Hello, World!</h1>
    <?php
        function hello() : int 
        {
            return 2;
        }
        var_dump(hello());
    ?>
</html>
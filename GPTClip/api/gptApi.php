<?php
if (isset($_GET['prompt']) && isset($_GET['pregunta'])) {
    $prompt = escapeshellarg($_GET['prompt']);
    $pregunta = escapeshellarg($_GET['pregunta']);

    $command = "/var/www/html/gptClip/.venv/bin/python /var/www/html/gptClip/duckduckgo-chat-ai/gpt.py $prompt $pregunta";
    
    $output = shell_exec($command);

    echo "$output";
}
?>

#!/bin/bash

/* Function to check if a command exists in the PATH */
command_exists() {
    type "$1" &> /dev/null
}

/* Main loop to read and execute commands */
while true; do
    /* Prompt for input */
    read -p ":) " CMD

    /* Check if the user wants to exit */
    if [ "$CMD" = "exit" ]; then
        echo "Exiting..."
        break
    fi

    /* Check if the command exists */
    if command_exists "$CMD"; then
        /* Execute the command */
        $CMD
    else
        /* Print an error message */
        echo "Command not found: $CMD"
    fi
done


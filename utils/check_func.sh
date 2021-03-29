#!/bin/bash

MAX_LINE_COUNT=25
EXPORTED_MAX=5
TOTAL_MAX=10
exit_code=0

add_file_error() {
    errors="$errors\e[39m- \e[31m$1\n"
    exit_code=1
}

color_string_on_gt_condition() { # value, max, string
    if [ "$1" -gt "$2" ]; then
        ret="\e[31m$3"
        exit_code=1
    else
        ret="\e[32m$3"
    fi
}

print_func() { # prototype, line count
    if [ "$2" -gt $MAX_LINE_COUNT ]; then
        function_errors="$function_errors   \e[33m$1 \e[31m($2 lines)\n"
        echo -ne "\e[31mF"
        exit_code=1
    else
        echo -ne "\e[32m."
    fi
    tput sgr0
}

check_file() {
    # Find function prototypes
    prots=$(ctags -x --c-types=f "$1" | sed -e 's/\s\+/ /g' | cut -d ' ' -f 5-)
    exported=$( echo "$prots" | grep -v ^.*static.* | wc -l )
    total=$( echo "$prots" | wc -l )

    # Remove empty lines
    file=$(grep -v -e '^[[:space:]]*$' -e '^[[:space:]]*//' -e '^[[:space:]]*/\*' -e '^[[:space:]]*\*/' "$1")
    line_count=$(echo "$file" | wc -l)

    # Function count (exported/total)
    color_string_on_gt_condition "$exported" "$EXPORTED_MAX" "$exported"
    exported_str="$ret"
    color_string_on_gt_condition "$total" "$TOTAL_MAX" "$total"
    total_str="$ret"
    if [ "$exported" -gt "$EXPORTED_MAX" ]; then
        add_file_error "Too many exported functions ($exported)"
    fi
    if [ "$total" -gt "$TOTAL_MAX" ]; then
        add_file_error "Too many functions ($total)"
    fi
    echo -ne "\e[39m($exported_str\e[39m/$total_str\e[39m) "
    tput sgr0

    echo -n "$1: "
    while read -r prot
    do
        # Find line number of function prototype
        line_nb=$(echo "$file" | grep -Fn "$prot" | cut -d : -f 1)

        # Trim file to keep only function body
        function_body=$(echo "$file" | tail -n +$line_nb)
        start_bracket=$(echo "$function_body" | grep -n -m 1 '^{$' | cut -d : -f 1)
        function_body=$(echo "$function_body" | tail -n +$(($start_bracket+1)))
        end_bracket=$(echo "$function_body" | grep -n -m 1 '^}$' | cut -d : -f 1)
        function_body=$(echo "$function_body" | head -n $(($end_bracket-1)))

        # Remove lines that do not count in the total line count
        function_body=$(echo "$function_body" | grep -v -e '^[[:space:]]*{$' -e '^[[:space:]]*}$')

        function_line_count=$(echo "$function_body" | wc -l)
        print_func "$prot" $function_line_count
    done < <(echo "$prots")
    echo

    if [ ! -z "$function_errors" ]; then
        add_file_error "Some functions are too long"
    fi
    if [ ! -z "$errors" ]; then
        echo -ne "$errors"
    fi
    echo -ne "$function_errors"
    tput sgr0
    unset errors
    unset function_errors
}

for f in $(find src -type f -name '*.c'); do
    check_file "$f"
done

exit "$exit_code"

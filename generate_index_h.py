# This script is used to convert the index.html file into a header file
# that can be used by the esp32 to serve a web page.
# I am doing this because it much easier to develop and debug the html when it is not
# embedded in char arrays in the esp32 code.

# This script is run as a pre-build step in platformio.

import os

Import("env")
print("=========== Pre Build ===============")

print("Converting html to header file...")
html_path = os.path.join(env['PROJECT_DIR'], 'espLamp', 'data', 'index.html')
header_path = os.path.join(env['PROJECT_DIR'], 'espLamp', 'src', 'index.h')

print('HTML path: ' + html_path)
print('Header path: ' + header_path)

with open(html_path, 'r') as file:
    html_content = file.read()

start_append = 'const char PAGE_MAIN[] PROGMEM = R"=====('
end_append = ')=====";'

modified_content = html_content.replace("<!DOCTYPE html>", start_append + ' <!DOCTYPE html>')
modified_content = modified_content.replace("</html>", '</html> ' + end_append)

# need to replace html comments with c comments
modified_content = modified_content.replace("<!--", "/*")
modified_content = modified_content.replace("-->", "*/")

with open(header_path, 'w') as file:
    file.write(modified_content)

print('Modified HTML file created successfully.')

print("===================================")
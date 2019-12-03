import os
import re  # regular expressions

docsFolder = "docs/docs/"
templatesFolder = "docs/templates/"

# the templates use placeholders for the name/details of each function/struct etc.
htmlTemplate = ""
htmlFunctionTemplate = ""

with open(templatesFolder + "template.html") as html:
    htmlTemplate = html.read()
with open(templatesFolder + "template_functions-structs.html") as functionTemplate:
    htmlFunctionTemplate = functionTemplate.read()

# will be concatenated with other regexes, which is why it isn't precompiled
commentRegex = r"(?:/\*\s?([\s\S]*?)\s?\*/\s*?)?"

# finds each comment/function pair and groups them in a list of tuples
functionAndCommentRegex = re.compile(
    commentRegex + r"(^\w.*\(.*\)[^;]?)", re.MULTILINE)

# finds each struct
structRegex = re.compile(
    commentRegex + r"^struct (\w+) {\s([\s\S]*?)\s};", re.MULTILINE)

# TODO: classes
# TODO: enums
# TODO: externs
# TODO: macros

# recursive function that applies the scanFile function to every .hpp
def searchTree(path):
    for file in os.listdir(path):
        currentPath = path + "/" + file
        if os.path.isdir(currentPath):
            searchTree(currentPath)
            continue
        if file.endswith(".hpp"):
            scanFile(currentPath, file)


# scans the entire file for comments, functions, structs, and documents them in an html
def scanFile(path, fileName):
    newFileName = fileName.replace(".hpp", ".html")
    htmlContent = ""
    with open(path) as f:
        fileContent = f.read().replace("\t", "")
        htmlContent += parseFunctionsAndComments(fileContent)
        htmlContent += parseStructs(fileContent)
    if htmlContent == "":
        return
    with open(docsFolder + newFileName, "w") as newHtml:
        newHtml.write(htmlTemplate.replace("<!--CONTENT-->", htmlContent))
        print("<a href='" + newFileName + "'>" +
              newFileName.replace(".html", "") + "</a>")


# finds all functions and comments (and also externally declared variables) and puts them into the html template
def parseFunctionsAndComments(fileContent):
    htmlFunctionContent = ""

    for match in functionAndCommentRegex.findall(fileContent):
        # match will contain the comment(s) in [0] and the function name in [1]
        comment = ""
        function = match[1]
        if function.startswith("extern "):
            continue
        for commentLine in re.split(r"[\n\r]", match[0]):
            # removes the asterisks from the beginning of each comment
            comment += re.sub(r"^[\*\s]*", "", commentLine).strip() + "<br>\n"
        htmlFunctionContent += htmlFunctionTemplate.replace(
            "<!--NAME-->", function).replace("<!--DETAILS-->", comment) + "\n"
    return htmlFunctionContent


# finds all structs and puts them into the html template
def parseStructs(fileContent):

    htmlStructsContent = ""

    if "struct " in fileContent:
        htmlStructsContent += "<h3>Structs</h3>\n"
        for struct in structRegex.findall(fileContent):
            # structComment: match[0], name: match[1], content: match[2]
            structComment = struct[0]
            structName = struct[1]
            structContent = struct[2].replace("\n", "<br>\n")
            # TODO: add struct comment
            htmlStructsContent += htmlFunctionTemplate.replace(
                "<!--NAME-->", structName).replace("<!--DETAILS-->", structContent) + "\n"
        return htmlStructsContent
    else:
        return ""


# deletes all documentation before writing new one
for x in os.listdir(docsFolder[0:len(docsFolder) - 1]):
    # these will not be deleted
    whitelist = ["index.html", "shadertool.html"]
    if x in whitelist:
        continue
    os.remove(docsFolder + x)

searchTree("TGEngine")

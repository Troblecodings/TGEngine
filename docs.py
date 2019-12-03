import os
import re  # regular expressions

docsFolder = "docs/docs/"
templatesFolder = "docs/templates/"

# the templates use placeholders for the name/details of each struct etc.
htmlTemplate = ""
htmlFunctionTemplate = ""

with open(templatesFolder + "template.html") as html:
    htmlTemplate = html.read()
with open(templatesFolder + "template_functions-structs.html") as functionTemplate:
    htmlFunctionTemplate = functionTemplate.read()


# finds each comment/function pair and groups them in a list of tuples
functionAndCommentRegex = re.compile(
    r"(?:/\*\s?([\s\S]*?)\s?\*/\s*?)?(^\w.*\(.*\)[^;]?)", re.MULTILINE)

# finds each struct
structRegex = re.compile(
    r"struct (\w+) {\s([\s\S]+)\s};", re.MULTILINE)


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
        function = match[0]
        if function.startswith("extern "):
            continue
        for commentLine in re.split(r"[\n\r]", match[1]):
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
            # structName: match[0], content: match[1]
            structName = struct[0]
            structContent = struct[1].replace("\n", "<br>\n")
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

# NOTE: currently breaks on classes (example: Properties.html) and does not consider enums right now (also Properties.html / .hpp)

searchTree("TGEngine")

import os
import re  # regular expressions


# used to group information, like a cpp struct
class cppGroupConstruct:  # group construct: classes, structs, enums
    comment = None
    name = None
    content = None


class cppDefinition:  # definition: externs, macros, functions
    comment = None
    definition = None


class fileContent:
    structs = []
    enums = []
    externs = []
    macros = []
    functions = []


# this order will also determine which order they appear in in the html
fileContentTypes = {
    "functions": "definition",
    "externs": "definition",
    "macros": "definition",
    "structs": "groupConstruct",
    "enums": "groupConstruct"
    # "class": "groupConstruct"
}


def initRegex():
    # structure: (comment)?(groupConstruct|definition)
    # all of the regexes, split into different strings for readability
    reComment = r"(?:/\*\s?(?P<comment>(?:[ *]+.*\s)*?)\s?\*/\s*?)?"

    reGroupConstruct = r"(?P<group>^(?P<groupType>class|struct|enum) (?P<groupName>\w+) (?:\: \w+ \w+ )?{\s(?P<groupContent>[\s\S]*?)\s};)"

    reExtern = r"(?P<extern>^extern .*)"
    reMacro = r"(?P<macro>^#define .*)"
    reFunction = r"(?P<function>^\w.*\(.*\);)"

    reDefinition = r"(?P<definition>" + \
        "|".join([reExtern, reMacro, reFunction]) + ")"

    reContent = r"(?P<content>" + \
        "|".join([reGroupConstruct, reDefinition]) + ")"

    # precompile to improve performance
    return re.compile(reComment + reContent, re.MULTILINE)


def readFile(path):
    with open(path) as file:
        return file.read()


def parsedFileContent(file):
    headerContent = fileContent()
    file = file.replace("\t", "")
    regexIterator = regularExpression.finditer(file)
    for match in regexIterator:
        # handle comments
        commentContent = None
        if match.group("comment") is not None:  # comments are optional
            commentContent = reCleanComment.sub("", match.group("comment"))

        # handle group constructs
        if match.group("group") is not None:  # is class, struct or enum
            construct = cppGroupConstruct()
            construct.comment = commentContent
            construct.name = match.group("groupName")
            construct.content = match.group("groupContent")
            groupType = match.group("groupType")
            if groupType == "class":
                continue  # classes are going to be removed anyway so there's no need to document them
            elif groupType == "struct":
                headerContent.structs.append(construct)
            elif groupType == "enum":
                headerContent.enums.append(construct)

        # handle definitions
        elif match.group("definition") is not None:  # is extern, macro or function
            definition = cppDefinition()
            definition.comment = commentContent
            definition.definition = match.group("definition")
            if match.group("extern") is not None:
                headerContent.externs.append(definition)
            elif match.group("macro") is not None:
                headerContent.macros.append(definition)
            elif match.group("function") is not None:
                headerContent.functions.append(definition)

    return fileContent


def convertHeaderContentToHTML(headerContent):
    htmlContent = ""

    for fileContentType in fileContentTypes:
        contentList = getattr(headerContent, fileContentType)

        if len(contentList) > 0:
            title = "<h3>" + fileContentType.title() + "</h3>\n"
            htmlContent += title

            if fileContentTypes[fileContentType] == "definition":
                template = htmlDefinitionTemplate
                for definition in contentList:
                    escapedAngleBracketsDefinition = definition.definition.replace(
                        "<", "&lt;").replace(">", "&gt;")
                    definitionHtml = template.replace(
                        "<!--DEFINITION-->", escapedAngleBracketsDefinition
                    )
                    if definition.comment is not None:
                        definitionHtml.replace(
                            "<!--COMMENT-->", definition.comment
                        )
                    htmlContent += definitionHtml + "\n"

            elif fileContentTypes[fileContentType] == "groupConstruct":
                template = htmlGroupConstructTemplate
                for groupConstruct in contentList:
                    groupConstructHtml = template.replace(
                        "<!--NAME-->", groupConstruct.name
                    ).replace(
                        "<!--CONTENT-->", groupConstruct.content
                    )
                    if groupConstruct.comment is not None:
                        groupConstructHtml.replace(
                            "<!--COMMENT-->", groupConstruct.comment)
                htmlContent += groupConstructHtml + "\n"

    return htmlContent


def generateDoc(path, fileName):
    newFileName = fileName.replace(".hpp", ".html")
    fileText = readFile(path)
    headerContent = parsedFileContent(fileText)
    newHtmlContent = convertHeaderContentToHTML(headerContent)

    newHtml = htmlTemplate.replace(
        "<!--PATH-->", path
    ).replace(
        "<!--CONTENT-->", newHtmlContent
    )

    with open(docsFolder + newFileName, "w") as newHtmlFile:
        newHtmlFile.write(newHtml)
        print("<a href='" + newFileName + "'>" +
              newFileName.replace(".html", "") + "</a>")


def searchTree(path):
    for file in os.listdir(path):
        currentPath = path + "/" + file
        if os.path.isdir(currentPath):
            searchTree(currentPath)
            continue
        if file.endswith(".hpp"):
            print("starting " + currentPath)
            generateDoc(currentPath, file)


def deleteDocs(path):
    # deletes all documentation before writing new one
    for x in os.listdir(docsFolder[0:len(docsFolder) - 1]):
        # these will not be deleted
        whitelist = ["index.html", "shadertool.html"]
        if x in whitelist:
            continue
        os.remove(docsFolder + x)


# matches all group constructs (classes, structs, enums), all macros, all externs and all functions
regularExpression = initRegex()
reCleanComment = re.compile(r"^[* ]+", re.MULTILINE)

docsFolder = "docs/docs/"
templatesFolder = "docs/templates/"

# the templates use placeholders in form of html comments for the name/details of each function/struct etc.
htmlTemplate = readFile(templatesFolder + "template.html")
htmlDefinitionTemplate = readFile(
    templatesFolder + "template_definitions.html")
htmlGroupConstructTemplate = readFile(
    templatesFolder + "template_group-constructs.html")

# -------------------------------------------

deleteDocs(docsFolder)
searchTree("TGEngine")

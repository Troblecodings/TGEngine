import os
import re  # regular expressions


# used to group information, like a cpp struct
class CppGroupConstruct:  # group construct: classes, structs, enums
    def __init__(self, comment: str, name: str, content: str):
        self.comment = comment or ""
        self.name = name
        self.content = content

    def toHtml(self) -> str:
        groupConstructHtml = dictionaryReplace(
            htmlGroupConstructTemplate,
            {
                "<!--NAME-->": self.name,
                "<!--CONTENT-->": self.content,
                "<!--COMMENT-->": self.comment
            }
        )

        return groupConstructHtml


class CppDefinition:  # definition: externs, macros, functions
    def __init__(self, comment: str, definition: str):
        self.comment = comment or ""
        self.definition = definition
        self.visibleDefinition = ""

    def toHtml(self) -> str:
        escapedDefinition = dictionaryReplace(
            self.visibleDefinition,
            {
                "<": "&lt;",
                ">": "&gt;"
            }
        )

        definitionHtml = dictionaryReplace(
            htmlDefinitionTemplate,
            {
                "<!--DEFINITION-->": escapedDefinition,
                "<!--COMMENT-->": self.comment
            }
        )

        return definitionHtml


class FileContent:
    def __init__(self):
        self.structs = []
        self.enums = []
        self.externs = []
        self.macros = []
        self.functions = []


def buildRegex() -> re.Pattern:
    # structure: (comment)?(groupConstruct|definition)
    # all of the regexes, split into different strings for readability
    reComment = r"(?:/\*\s?(?P<comment>(?:[ *]+.*\s)*?)\s?\*/\s*?)?"

    reGroupConstruct = r"(?P<group>^(?P<groupType>class|struct|enum) (?P<groupName>\w+) (?:\: \w+ \w+ )?{\s(?P<groupContent>[\s\S]*?)\s};)"

    reExtern = r"(?P<extern>^extern (?P<externVisible>.*))"
    reMacro = r"(?P<macro>^#define (?P<macroVisible>\w+) .*)"
    reFunction = r"(?P<function>(?P<functionVisible>^\w.*\(.*\);))"

    reDefinition = r"(?P<definition>" + \
        "|".join([reExtern, reMacro, reFunction]) + ")"
    reContent = r"(?P<content>" + \
        "|".join([reGroupConstruct, reDefinition]) + ")"

    # precompile to improve performance
    # full regex: r"(?:/\*\s?(?P<comment>(?:[ *]+.*\s)*?)\s?\*/\s*?)?(?P<content>(?P<group>^(?P<groupType>class|struct|enum) (?P<groupName>\w+) (?:\: \w+ \w+ )?{\s(?P<groupContent>[\s\S]*?)\s};)|(?P<definition>(?P<extern>^extern (?P<externVisible>.*))|(?P<macro>^#define (?P<macroVisible>\w+) .*)|(?P<function>(?P<functionVisible>^\w.*\(.*\);))))"
    return re.compile(reComment + reContent, re.MULTILINE)


def readFile(path: str) -> str:
    with open(path) as file:
        return file.read()


def parseFileContentToObject(fileContent: str) -> FileContent:
    headerContent = FileContent()
    fileContent = re.sub(r"^\s+", "", fileContent, flags=re.MULTILINE)
    regexIterator = regularExpression.finditer(fileContent)

    for match in regexIterator:
        # handle comments
        commentContent = None
        if match.group("comment") is not None:  # comments are optional
            commentContent = reCleanComment.sub(
                "", match.group("comment")).replace("\n", "<br>")

        # handle group constructs
        if match.group("group") is not None:  # is class, struct or enum
            construct = CppGroupConstruct(commentContent, match.group(
                "groupName"), match.group("groupContent"))
            groupType = match.group("groupType")

            if groupType == "class":
                continue  # classes are going to be removed anyway so there's no need to document them

            elif groupType == "struct":
                headerContent.structs.append(construct)

            elif groupType == "enum":
                headerContent.enums.append(construct)

        # handle definitions
        elif match.group("definition") is not None:  # is extern, macro or function
            definition = CppDefinition(
                commentContent, match.group("definition"))

            if match.group("extern") is not None:
                definition.visibleDefinition = match.group("externVisible")
                headerContent.externs.append(definition)

            elif match.group("macro") is not None:
                definition.visibleDefinition = match.group("macroVisible")
                headerContent.macros.append(definition)

            elif match.group("function") is not None:
                definition.visibleDefinition = match.group("functionVisible")
                headerContent.functions.append(definition)

    return headerContent


def convertHeaderContentToHTML(headerContent: FileContent) -> str:
    htmlContent = ""

    for fileContentType in fileContentTypes:
        contentList = getattr(headerContent, fileContentType)

        if len(contentList) > 0:
            title = f"<h3>{fileContentType.title()}</h3>\n"
            htmlContent += title

            if fileContentTypes[fileContentType] == "definition":
                for definition in contentList:
                    htmlContent += definition.toHtml() + "\n"

            elif fileContentTypes[fileContentType] == "groupConstruct":
                for groupConstruct in contentList:
                    htmlContent += groupConstruct.toHtml() + "\n"

    return htmlContent


def generateDoc(path: str, fileName: str) -> None:
    newFileName = fileName.replace(".hpp", ".html")
    fileText = readFile(path)
    headerContent = parseFileContentToObject(fileText)
    newHtmlContent = convertHeaderContentToHTML(headerContent)

    newHtml = dictionaryReplace(
        htmlTemplate, {"<!--PATH-->": path, "<!--CONTENT-->": newHtmlContent})
    with open(os.path.join(docsFolder, newFileName), "w") as newHtmlFile:
        newHtmlFile.write(newHtml)
        print(
            f"<a href='{newFileName}'>{newFileName.replace('.html', '')}</a>")


def searchTree(path: str) -> None:
    for file in os.listdir(path):
        currentPath = os.path.join(path, file)
        if os.path.isdir(currentPath):
            searchTree(currentPath)
            continue
        if file.endswith(".hpp"):
            generateDoc(currentPath, file)


def deleteDocs(path: str) -> None:
    # deletes all documentation before writing new one
    for x in os.listdir(docsFolder[0:len(docsFolder) - 1]):
        # these will not be deleted
        whitelist = ["index.html", "shadertool.html"]
        if x in whitelist:
            continue
        os.remove(docsFolder + x)


def dictionaryReplace(string: str, dictionary: dict) -> str:
    for item in dictionary:
        string = string.replace(item, dictionary[item])
    return string


# this order will also determine which order they appear in in the html
fileContentTypes = {
    "functions": "definition",
    "externs": "definition",
    "macros": "definition",
    "structs": "groupConstruct",
    "enums": "groupConstruct"
    # "class": "groupConstruct"
}

# matches all group constructs (classes, structs, enums), all macros, all externs and all functions
regularExpression = buildRegex()
reCleanComment = re.compile(r"^[* ]+", re.MULTILINE)

docsFolder = "Documentations/docs/"
templatesFolder = "Documentations/templates/"

# the templates use placeholders in form of html comments for the name/details of each function/struct etc.
htmlTemplate = readFile(os.path.join(templatesFolder, "template.html"))
htmlDefinitionTemplate = readFile(os.path.join(
    templatesFolder, "template_definitions.html"))
htmlGroupConstructTemplate = readFile(os.path.join(
    templatesFolder, "template_group-constructs.html"))

if __name__ == "__main__":
    deleteDocs(docsFolder)
    searchTree("TGEngine")

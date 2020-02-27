import yaml
import io
import os
import re

def dirFileSearch(path):
   fileCount, fileStructure, installPaths = 0, [], []

   for root, _, filenames in os.walk(path):
      installPath = root[len(path):]

      if len(installPath) > 0:
         installPath = '.\\' + installPath[1:] + '\\'
      else:
         installPath = '.\\'

      list(map(lambda fileName: fileStructure.append(fileName), list(map(lambda fileName: root + '\\' + fileName, filenames))))
      list(map(lambda fileName: installPaths.append(fileName), list(map(lambda fileName: installPath + fileName, filenames))))
      fileCount += len(filenames)

   return fileStructure, fileCount, installPaths

def dirRecursiveDelete(path):
   for root, dirs, files in os.walk(path, topdown=False):
      for name in files:
        os.remove(os.path.join(root, name))
      for name in dirs:
        os.rmdir(os.path.join(root, name))

   os.rmdir(path)

def normalizeVariableName(name):
   res = re.sub(r'[^a-zA-Z\d]', '_', name)
   if res[0].isnumeric():
      res = '_' + res

   return res

def createVariableDefinition(categoryName, filePath, resourceDefs, installPath):
   varName = normalizeVariableName(installPath)
   fileString = f'const unsigned char {varName}' + '[] = {\n'
   fileSize = 0

   with io.open(filePath, mode='rb') as file:

      while (True):
         byteArr = file.read(20)

         if len(byteArr) < 1:
            break     

         fileSize += len(byteArr)
         hexString = '    '
         for byte in byteArr:
            hexString += '0x{0:02X}, '.format(byte)

         fileString += hexString + '\n'

   fileString = fileString[: -3] + '\n};\n\n'
   resourceDefs.append((varName, fileSize, installPath))

   return fileString

def createResourceDefinitions(category, resourceDefs):
   fileString = f'const MemResource Resources::{category}::resources' + '[] = {\n'

   for varName, size, path in resourceDefs:
      fileString += ' ' * 4 + '{' + f'{varName}, {size}, "' + path.replace('\\', '\\\\') + '"},\n'

   return fileString[:-2] + '\n};'

def writeHeaderFile(fileStr):
   with io.open(includeFolder + '\\resources.hpp', mode='w', encoding='utf-8') as file:
      file.write(fileStr)

class category:
   def __init__(self, name, resourcesConf):
      self.name = normalizeVariableName(name)
      self.fileStructure = []

      if resourcesConf:
         if resourcesConf['files']:
            for file in resourcesConf['files']:
               self.fileStructure.append( (file, '.\\' + file.split('\\')[-1].split('/')[-1]) )

         if resourcesConf['directories']:
            for directory in resourcesConf['directories']:
               files, count, installPaths = dirFileSearch(directory)
               for i in range(0, len(installPaths)):
                  self.fileStructure.append( (files[i], installPaths[i]) )



resourcesConf = None

with io.open('.\\resources.yaml', mode='r', encoding='utf-8') as file:
   resourcesConf = yaml.load(file, Loader=yaml.FullLoader)

if not os.path.exists('.\\build'):
   os.makedirs('.\\build')
else:
   dirRecursiveDelete('.\\build')
   os.makedirs('.\\build')

includeFolder = '.\\build\\include'
if not os.path.exists(includeFolder):
   os.makedirs(includeFolder)

sourceFolder = '.\\build\\source'
if not os.path.exists(sourceFolder):
   os.makedirs(sourceFolder)

categories = []

if resourcesConf and resourcesConf['categories']:
   for categoryConf in resourcesConf['categories']:
      categories.append(category(categoryConf, resourcesConf['categories'][categoryConf]))

hFileStr =  '#ifndef RESOURCES_HEADER\n'
hFileStr += '#define RESOURCES_HEADER\n'
hFileStr += '\n\
struct MemResource {\n\
    const unsigned char *data;\n\
    const unsigned long long size;\n\
    const char *path;\n\
};\n\
\n\
namespace Resources {\n'

for category in categories:
   print(f'Processing category: [{category.name}]')
   if len(category.fileStructure) < 1:
      print('[----] category is empty')
      continue

   with io.open(sourceFolder + f'\\{category.name}.cpp', mode='w', encoding='utf-8') as codeFile:
      codeFile.write('#include "../include/resources.hpp"\n\n')
      hFileStr += ' ' * 4 + f'namespace {category.name}' + ' {\n' + ' ' * 8 + 'enum names {\n'
      index, varNames, resourceDefs = 1, ['resources', 'count', 'names'], []
      successful = 0

      for filePath, installPath in category.fileStructure:
         fileName = installPath.split('\\')[-1]
         variableName = normalizeVariableName(fileName.split('.')[0])

         if not variableName in varNames:
            varNames.append(variableName)
         else:
            variableName = None

         statusString = str(round(index / len(category.fileStructure) * 100))
         statusString = '[' + ' ' * (3 - len(statusString)) + statusString + f'%] processing file {fileName} ...\b\b\b'
         print(statusString, end='')

         if not os.path.exists(filePath):
            print(':: file not found')
         elif os.path.getsize(filePath) < 1:
            print(':: file is empty')
         else:
            try:
               codeFile.write(createVariableDefinition(category.name, filePath, resourceDefs, installPath))

               if (variableName):
                  hFileStr += ' ' * 12 + variableName + f' = {successful}' + ',\n'

               successful += 1

               print(':: success')
            except:
               print(':: error in processing')

         index += 1

      codeFile.write(createResourceDefinitions(category.name, resourceDefs))

   hFileStr = hFileStr[:-2] + '\n' + ' ' * 8 + '};\n\n'
   hFileStr += ' ' * 8 + f'const long count = {successful};\n'
   hFileStr += ' ' * 8 + 'extern const MemResource resources[];\n' + ' ' * 4 + '}\n\n'

hFileStr = hFileStr[:-1] + '}\n\n#endif'
writeHeaderFile(hFileStr)
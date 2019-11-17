import os

class ShadowInstaller:
   def __init__(self, installDir = None):
      if installDir == None:
         self.installDir = os.path.expanduser('~') + '\\AppData\\Roaming\\Discord\\0.0.305\\modules\\discord_desktop_core'

   def findSrcFiles(self):
      fileCount = 0
      fileStructure = []

      for root, _, filenames in os.walk('.'):
         fileStructure.append((root, filenames))
         fileCount += len(filenames)

      return fileStructure, fileCount

   def installFiles(self, fileStructure, fileCount):
      from shutil import copy as copyFile

      for (folder, files) in fileStructure:
         if not os.path.exists(self.installDir + folder[1:]):
            os.makedirs(self.installDir + folder[1:])

         for file in files:
            fileName = os.path.join(folder, file)
            copyFile(os.path.join(fileName), self.installDir + folder[1:])

   def findAndInstall(self):
      os.chdir('.\\src')
      fileStructure, fileCount = self.findSrcFiles()
      self.installFiles(fileStructure, fileCount)

def main():
   ShadowInstaller().findAndInstall()

if __name__ == '__main__':
   main()
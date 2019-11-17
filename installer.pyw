import os
import time
import subprocess
import sys
from threading import Thread

try:
   import wx
except:
   process = subprocess.Popen([os.path.dirname(sys.executable) + '\\python.exe', '-m', 'pip', 'install', '-U', '--pre', '-f', 'file:///' + os.path.join(os.getcwd(), 'resources'), 'wxpython'], shell=False)
   process.wait()
   import wx

from wx.lib.newevent import NewEvent

discordDir = os.path.expanduser('~') + '\\AppData\\Roaming\\Discord\\0.0.305\\modules\\discord_desktop_core'
colors = {
   'frame':            wx.Colour(  0,  71,  76),
   'background':       wx.Colour( 32,  34,  37),
   'content':          wx.Colour( 53,  57,  63),
   'hover-red':        wx.Colour(240,  71,  71),
   'hover-light':      wx.Colour( 42,  44,  48),
   'main-text':        wx.Colour(223, 255, 247),
   'secondary-text':   wx.Colour(134, 154, 149),
   'text-input':       wx.Colour( 64,  68,  75),
   'button':           wx.Colour(185, 187, 190),
   'button-hover':     wx.Colour(152, 218, 196),
   'progress-pending': wx.Colour( 18,  51,  46),
   'progress':         wx.Colour( 72, 205, 183)
}

class ScaledBitmap (wx.Bitmap):
   def __init__(self, path, width, height):
      image = wx.Image(name = path, type = wx.BITMAP_TYPE_ANY, index = -1)

      ratio = min(
         float(width) / float(image.GetWidth()),
         float(height) / float(image.GetHeight())
      )

      image = image.Scale(
         round(float(image.GetWidth()) * ratio),
         round(float(image.GetHeight()) * ratio),
         wx.IMAGE_QUALITY_HIGH
      )

      wx.Bitmap.__init__(self, image.ConvertToBitmap())

class ActionButton (wx.Button):
   def __init__(self, parent, **kwargs):
      wx.Button.__init__(self, parent, **kwargs)

      self.Initialize()

   def Initialize(self):
      if self.passiveColor != None:
         self.SetBackgroundColour(self.passiveColor)

      self.Bind(wx.EVT_ENTER_WINDOW, self.onMouseEnter)
      self.Bind(wx.EVT_LEAVE_WINDOW, self.onMouseLeave)
      self.Bind(wx.EVT_BUTTON, self.onClick)

   def onMouseEnter(self, event):
      if self.hoverColor != None:
         self.SetBackgroundColour(self.hoverColor)

   def onMouseLeave(self, event):
      if self.passiveColor != None:
         self.SetBackgroundColour(self.passiveColor)

   def onClick(self, event):
      self.PerformAction()

   def PerformAction(self):
      raise NotImplementedError('Method \'PerformAction\' is not implemented')

class ActionBitmapButton (wx.BitmapButton, ActionButton):
   def __init__(self, parent, **kwargs):
      wx.BitmapButton.__init__(self, parent, **kwargs)

      self.Initialize()

class DirPickerButton (ActionButton):
   def __init__(self, parent, size, pos):
      self.passiveColor = colors['button']
      self.hoverColor = colors['button-hover']
      self.OnDirSelected = None

      ActionBitmapButton.__init__(
         self,
         parent,
         size = size,
         style = wx.BORDER_NONE,
         pos = pos,
         label='Select Folder',
      )

      self.SetForegroundColour(colors['hover-light'])
      font = wx.Font(10, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_BOLD, faceName = 'Roboto Slab')
      self.SetFont(font)

   def PerformAction(self):
      dialog = wx.DirDialog(
         self,
         message='Select Folde',
         defaultPath=discordDir,
         style=wx.DD_DEFAULT_STYLE,
         pos=wx.DefaultPosition,
         size=wx.DefaultSize
      )
      dialog.ShowModal()

      if callable(self.OnDirSelected):
         self.OnDirSelected(dialog.GetPath())

class FolderSelect (wx.Panel):
   def __init__(self, parent):
      self.ContainerWidth = 536
      wx.Panel.__init__(self, parent, size = wx.Size(self.ContainerWidth, 210))

      self.SetBackgroundColour(colors['hover-light'])
      self.CreateElements()

   def CreateElements(self):
      self.CreateTextInput()
      self.CreatePathLabel()
      self.CreateInstruction()
      self.CreateBrowseButton()

   def CreateBrowseButton(self):
      button = DirPickerButton(self, pos = (self.ContainerWidth - 120, 140), size = (100, 30))
      button.OnDirSelected = self.SelectDir

   def CreateInstruction(self):
      font = wx.Font(11, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL, faceName = 'Roboto Slab')
      text = wx.StaticText(self, label = 'Select Discord\'s core module folder:', pos=(20, 40))
      text.SetForegroundColour(colors['main-text'])
      text.SetFont(font)

   def CreatePathLabel(self):
      font = wx.Font(11, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL, faceName = 'Roboto Slab')
      text = wx.StaticText(self, label = 'Folder:', pos=(20, 92))
      text.SetForegroundColour(colors['secondary-text'])
      text.SetFont(font)

   def CreateTextInput(self):
      textPanelWidth = self.ContainerWidth - 95
      panel = wx.Panel(self, size = wx.Size(textPanelWidth, 26), pos=(75, 90))
      panel.SetBackgroundColour(colors['text-input'])
      font = wx.Font(10, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL, faceName = 'Roboto Slab')

      self.textInput = wx.TextCtrl(panel, value=discordDir, pos=(6, 3), size=(textPanelWidth - 12, 20), style=wx.NO_BORDER)
      self.textInput.SetBackgroundColour(colors['text-input'])
      self.textInput.SetForegroundColour(colors['main-text'])
      self.textInput.SetFont(font)

   def SelectDir(self, path):
      global discordDir
      discordDir = path
      self.textInput.SetValue(discordDir)

class InstallButton (ActionButton):
   def __init__(self, parent, size, pos):
      self.passiveColor = colors['button']
      self.hoverColor = colors['button-hover']

      ActionBitmapButton.__init__(
         self,
         parent,
         size = size,
         style = wx.BORDER_NONE,
         pos = pos,
         label='Install',
      )

      self.SetForegroundColour(colors['content'])
      font = wx.Font(10, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_BOLD, faceName = 'Roboto Slab')
      self.SetFont(font)

   def PerformAction(self):
      window.BeginInstall()

class TitleImage (wx.StaticBitmap):
   def __init__(self, parent):
      logo  = ScaledBitmap('./resources/logo.png', 587, 24)

      wx.StaticBitmap.__init__(
         self,
         parent,
         size = wx.Size(logo.GetWidth(), logo.GetHeight()),
         bitmap = logo,
         pos = (2, 0)
      )

      self.Bind(wx.EVT_LEFT_UP, parent.OnLeftUp)
      self.Bind(wx.EVT_MOTION, parent.OnMouseMove)
      self.Bind(wx.EVT_LEFT_DOWN, parent.OnLeftDown)

class CloseButton (ActionBitmapButton):
   def __init__(self, parent):
      self.passiveColor = colors['background']
      self.hoverColor = colors['hover-red']

      ActionBitmapButton.__init__(
         self,
         parent,
         bitmap = ScaledBitmap('./resources/close.png', 42, 24),
         size = wx.Size(42, 24),
         style = wx.BORDER_NONE,
         pos = (658, 0)
      )

   def PerformAction(self):
      if (window.ReadyToClose):
         window.Close()
      else:
         answer = wx.MessageBox(
            "Are you sure you want to stop the installation?",
            "Exit Installation",
            wx.YES_NO | wx.ICON_EXCLAMATION,
            window
         )
         if answer == wx.YES:
            window.Close()

class MinimizeButton (ActionBitmapButton):
   def __init__(self, parent):
      self.passiveColor = colors['background']
      self.hoverColor = colors['hover-light']

      ActionBitmapButton.__init__(
         self,
         parent,
         bitmap = ScaledBitmap('./resources/minimize.png', 42, 24),
         size = wx.Size(42, 24),
         style = wx.BORDER_NONE,
         pos = (616, 0)
      )

   def PerformAction(self):
      window.Iconize()

class TitlePannel (wx.Panel):
   def __init__(self, parent):
      wx.Panel.__init__(self, parent, size = wx.Size(700, 24))

      self.delta = None
      self.SetBackgroundColour(colors['background'])

      self.Bind(wx.EVT_LEFT_UP, self.OnLeftUp)
      self.Bind(wx.EVT_MOTION, self.OnMouseMove)
      self.Bind(wx.EVT_LEFT_DOWN, self.OnLeftDown)
      self.addPanelControls()

   def addPanelControls(self):
      TitleImage(self)
      CloseButton(self)
      MinimizeButton(self)

   def OnLeftDown(self, event):
      self.CaptureMouse()
      pos = window.ClientToScreen(event.GetPosition())
      origin = window.GetPosition()
      self.delta = wx.Point(pos.x - origin.x, pos.y - origin.y)

   def OnMouseMove(self, event):
      if event.Dragging() and event.LeftIsDown() and self.delta != None:
         pos = window.ClientToScreen(event.GetPosition())
         newPos = (pos.x - self.delta.x, pos.y - self.delta.y)
         window.Move(newPos)

   def OnLeftUp(self, event):
      if self.HasCapture():
         self.ReleaseMouse()

class ContentPannel (wx.Panel):
   def __init__(self, parent):
      self.ContainerWidth = 536
      wx.Panel.__init__(self, parent, size = wx.Size(self.ContainerWidth, 451), pos = (164, 24))

      self.SetBackgroundColour(colors['content'])
      self.CreateContent()

   def CreateContent(self):
      raise NotImplementedError('Method \'CreateContent\' is not implemented')

   def CreateFormatedString(self, padding, pieces):
      stringSizer = wx.BoxSizer(orient=wx.HORIZONTAL)
      stringSizer.AddSpacer(padding)

      for text, fontWeight, fontSize, fontStyle, height, color in pieces:
         font = wx.Font(fontSize, wx.FONTFAMILY_DEFAULT, fontStyle, fontWeight, faceName = 'Roboto Slab')
         text = wx.StaticText(self, size = wx.Size(-1, height), label = text)
         text.SetForegroundColour(color)
         text.SetFont(font)

         stringSizer.Add(text, 0, wx.ALIGN_BOTTOM)

      return stringSizer

   def CreateSeparator(self):
      return wx.StaticBitmap(self, bitmap=ScaledBitmap('./resources/separator.png', 500, 6), size = (500, 6))

class InstallationStartPanel (ContentPannel):
   def __init__(self, parent):
      ContentPannel.__init__(self, parent)

   def CreateContent(self):
      contentSizer = wx.BoxSizer(wx.VERTICAL)

      contentSizer.AddSpacer(30)
      contentSizer.Add(self.CreateTitleText(), 0)
      contentSizer.AddSpacer(40)
      contentSizer.Add(self.CreateInstructionsText(), 0, wx.EXPAND)
      contentSizer.AddSpacer(30)
      contentSizer.Add(FolderSelect(self), 0, wx.EXPAND)

      self.SetSizer(contentSizer)
      self.Layout()

      InstallButton(self, size = (100, 30), pos = (415, 408))

   def CreateTitleText(self):
      linePieces = [
         ('Install ', wx.FONTWEIGHT_LIGHT, 20, wx.FONTSTYLE_NORMAL, 40, colors['main-text']),
         (' Pepega-Inject ', wx.FONTWEIGHT_BOLD, 20, wx.FONTSTYLE_NORMAL, 40, colors['main-text']),
         (' ' + versionNumber, wx.FONTWEIGHT_LIGHT, 14, wx.FONTSTYLE_NORMAL, 32, colors['main-text'])
      ]

      return self.CreateFormatedString(20, linePieces)

   def CreateInstructionsText(self):
      instructionsSizer = wx.BoxSizer(orient=wx.VERTICAL)

      linePieces = [
         ('Select the direcory where the', wx.FONTWEIGHT_NORMAL, 11, wx.FONTSTYLE_NORMAL, -1, colors['main-text']),
         (' Pepega Inject ', wx.FONTWEIGHT_BOLD, 11, wx.FONTSTYLE_NORMAL, -1, colors['main-text']),
         ('should be installed to.', wx.FONTWEIGHT_NORMAL, 11, wx.FONTSTYLE_NORMAL, -1, colors['main-text'])
      ]
      instructionsSizer.Add(self.CreateFormatedString(20, linePieces), 0)

      linePieces = [
         ('The folder should contain \'', wx.FONTWEIGHT_NORMAL, 11, wx.FONTSTYLE_NORMAL, -1, colors['secondary-text']),
         ('index.js', wx.FONTWEIGHT_NORMAL, 11, wx.FONTSTYLE_ITALIC, -1, colors['secondary-text']),
         ('\' scirpt and \'', wx.FONTWEIGHT_NORMAL, 11, wx.FONTSTYLE_NORMAL, -1, colors['secondary-text']),
         ('core.asar', wx.FONTWEIGHT_NORMAL, 11, wx.FONTSTYLE_ITALIC, -1, colors['secondary-text']),
         ('\' archive files.', wx.FONTWEIGHT_NORMAL, 11, wx.FONTSTYLE_NORMAL, -1, colors['secondary-text'])
      ]
      instructionsSizer.Add(self.CreateFormatedString(20, linePieces), 0)

      return instructionsSizer

class AbortButton (ActionButton):
   def __init__(self, parent, size, pos):
      self.passiveColor = colors['button']
      self.hoverColor = colors['hover-red']

      ActionBitmapButton.__init__(
         self,
         parent,
         label = 'Abort',
         size = size,
         style = wx.BORDER_NONE,
         pos = pos
      )

      self.SetForegroundColour(colors['content'])
      font = wx.Font(10, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_BOLD, faceName = 'Roboto Slab')
      self.SetFont(font)

   def PerformAction(self):
      if (window.ReadyToClose):
         window.Close()
      else:
         answer = wx.MessageBox(
            "Are you sure you want to stop the installation?",
            "Abort Installation",
            wx.YES_NO | wx.ICON_EXCLAMATION,
            window
         )
         if answer == wx.YES:
            window.Close()

class ProgressBar (wx.Panel):
   def __init__(self, parent, pos, size):
      self.barWidth, self.barHeight = size
      wx.Panel.__init__(self, parent, size = size, pos = pos)

      self.SetBackgroundColour(colors['content'])

      self.CreateMovablePanles()

   def CreateMovablePanles(self):
      wx.Panel(self, size = (self.barWidth -  6, self.barHeight -  6), pos = (3, 3)).SetBackgroundColour(colors['background'])
      wx.Panel(self, size = (self.barWidth - 10, self.barHeight - 10), pos = (5, 5)).SetBackgroundColour(colors['progress-pending'])

      self.leftPanel = wx.Panel(self, size = (0, 0), pos = (5, 5))
      self.leftPanel.SetBackgroundColour(colors['progress'])

   def SetProgress(self, value):
      if value < 0:
         value = 0
      if value > 1:
         value = 1

      self.leftPanel.SetSize(round((self.barWidth - 10) * value), self.barHeight - 10)

class InstallProgress (wx.Panel):
   def __init__(self, parent):
      self.ContainerWidth = 536
      wx.Panel.__init__(self, parent, size = wx.Size(self.ContainerWidth, 210))

      self.SetBackgroundColour(colors['hover-light'])
      self.CreateElements()

   def findSrcFiles(self):
      fileCount = 0
      fileStructure = []

      for root, _, filenames in os.walk('.'):
         fileStructure.append((root, filenames))
         fileCount += len(filenames)

      return fileStructure, fileCount

   def installFiles(self, fileStructure, fileCount):
      from shutil import copy as copyFile

      done = 0
      for (folder, files) in fileStructure:
         if not os.path.exists(discordDir + folder[1:]):
            os.makedirs(discordDir + folder[1:])

         for file in files:
            fileName = os.path.join(folder, file)
            self.fileName.SetLabel(fileName)
            copyFile(os.path.join(fileName), discordDir + folder[1:])
            done += 1
            self.progressBar.SetProgress(float(done) / float(fileCount))

   def findAndInstall(self):
      self.actionText.SetLabel('Searching installation files ...')
      os.chdir('.\\src')
      fileStructure, fileCount = self.findSrcFiles()
      time.sleep(0.25)

      self.actionText.SetLabel('Copying files:')
      self.installFiles(fileStructure, fileCount)
      time.sleep(0.25)

      wx.PostEvent(self, self.InstallFinishedEvent(data=None))

   def onInstallFinished(self, event):
      self.installThread.join()
      window.InstallSucess()

   def BeginInstallation(self):
      self.InstallFinishedEvent, EVT_RESULT = NewEvent()
      self.Bind(EVT_RESULT, self.onInstallFinished)

      self.installThread = Thread(target=self.findAndInstall)
      self.installThread.start()

   def CreateElements(self):
      self.CreateLabel()
      self.CreateStatusText()
      self.CreateProgressBar()

   def CreateLabel(self):
      font = wx.Font(11, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL, faceName = 'Roboto Slab')
      self.actionText = wx.StaticText(self, label = ' ', pos=(20, 50))
      self.actionText.SetForegroundColour(colors['main-text'])
      self.actionText.SetFont(font)

   def CreateStatusText(self):
      font = wx.Font(11, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_NORMAL, faceName = 'Roboto Slab')
      self.fileName = wx.StaticText(self, label = ' ', pos=(40, 80), size = (self.ContainerWidth - 60, -1))
      self.fileName.SetForegroundColour(colors['secondary-text'])
      self.fileName.SetFont(font)

   def CreateProgressBar(self):
      self.progressBar = ProgressBar(self, size = (self.ContainerWidth - 40, 32), pos=(20, 120))

class InstallForm (ContentPannel):
   def __init__(self, parent):
      ContentPannel.__init__(self, parent)

   def CreateContent(self):
      contentSizer = wx.BoxSizer(wx.VERTICAL)

      contentSizer.AddSpacer(30)
      contentSizer.Add(self.CreateTitleText(), 0)
      contentSizer.AddSpacer(40)
      contentSizer.Add(self.CreateInfoText(), 0, wx.EXPAND)
      contentSizer.AddSpacer(30)
      self.installProcess = InstallProgress(self)
      contentSizer.Add(self.installProcess, 0, wx.EXPAND)

      self.SetSizer(contentSizer)
      self.Layout()

      AbortButton(self, size = (100, 30), pos = (415, 408))

   def CreateTitleText(self):
      linePieces = [
         ('Installing ', wx.FONTWEIGHT_LIGHT, 20, wx.FONTSTYLE_NORMAL, 40, colors['main-text']),
         ('Pepega-Inject ', wx.FONTWEIGHT_BOLD, 20, wx.FONTSTYLE_NORMAL, 40, colors['main-text']),
         ('...', wx.FONTWEIGHT_LIGHT, 20, wx.FONTSTYLE_NORMAL, 40, colors['main-text']),
      ]

      return self.CreateFormatedString(20, linePieces)

   def CreateInfoText(self):
      linePieces = [
         ('Please wait while the injector is being installed.', wx.FONTWEIGHT_NORMAL, 11, wx.FONTSTYLE_NORMAL, 30, colors['main-text']),
      ]

      return self.CreateFormatedString(20, linePieces)

   def BeginInstallation(self):
      self.installProcess.BeginInstallation()

class ExitInstallerButton (ActionButton):
   def __init__(self, parent, size, pos):
      self.passiveColor = colors['button']
      self.hoverColor = colors['button-hover']

      ActionButton.__init__(
         self,
         parent,
         size = size,
         pos = pos,
         label = 'Finish',
         style = wx.BORDER_NONE
      )

      self.SetForegroundColour(colors['content'])
      font = wx.Font(10, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_BOLD, faceName = 'Roboto Slab')
      self.SetFont(font)

   def PerformAction(self):
      if (window.ReadyToClose):
         window.Close()
      else:
         answer = wx.MessageBox(
            "Are you sure you want to exit the installation?",
            "Exit Installation",
            wx.YES_NO | wx.ICON_EXCLAMATION,
            window
         )
         if answer == wx.YES:
            window.Close()

class InstallSucessPanel (ContentPannel):
   def __init__(self, parent):
      ContentPannel.__init__(self, parent)

   def CreateContent(self):
      contentSizer = wx.BoxSizer(wx.VERTICAL)

      contentSizer.AddSpacer(30)
      contentSizer.Add(self.CreateTitleText(), 0)
      contentSizer.AddSpacer(40)
      contentSizer.Add(self.CreateInfoText(), 0, wx.EXPAND)

      self.SetSizer(contentSizer)
      self.Layout()

      ExitInstallerButton(self, size = (100, 30), pos = (415, 408))

   def CreateTitleText(self):
      linePieces = [
         ('Installation Complete', wx.FONTWEIGHT_LIGHT, 20, wx.FONTSTYLE_NORMAL, 40, colors['main-text'])
      ]

      return self.CreateFormatedString(20, linePieces)

   def CreateInfoText(self):
      infoSizer = wx.BoxSizer(wx.VERTICAL)

      linePieces = [
         ('The ', wx.FONTWEIGHT_NORMAL, 11, wx.FONTSTYLE_NORMAL, 30, colors['main-text']),
         ('Pepega Inject ', wx.FONTWEIGHT_BOLD, 11, wx.FONTSTYLE_NORMAL, 30, colors['main-text']),
         ('was successfully installed', wx.FONTWEIGHT_NORMAL, 11, wx.FONTSTYLE_NORMAL, 30, colors['main-text']),
      ]
      infoSizer.Add(self.CreateFormatedString(20, linePieces), 0)

      infoSizer.AddSpacer(30)

      linePieces = [
         ('You may now exist the installer. Be sure to restart the ', wx.FONTWEIGHT_NORMAL, 11, wx.FONTSTYLE_NORMAL, 30, colors['secondary-text']),
         ('Discord', wx.FONTWEIGHT_BOLD, 11, wx.FONTSTYLE_ITALIC, 30, colors['secondary-text'])
      ]
      infoSizer.Add(self.CreateFormatedString(20, linePieces), 0)

      linePieces = [
         ('Application ', wx.FONTWEIGHT_BOLD, 11, wx.FONTSTYLE_ITALIC, 30, colors['secondary-text']),
         ('to apply the changes', wx.FONTWEIGHT_NORMAL, 11, wx.FONTSTYLE_NORMAL, 30, colors['secondary-text']),
      ]
      infoSizer.Add(self.CreateFormatedString(20, linePieces), 0)

      return infoSizer

class MainWindow (wx.Frame):
   def __init__(self, parent):
      wx.Frame.__init__(
         self,
         parent,
         id = wx.ID_ANY,
         title = wx.EmptyString,
         pos = wx.DefaultPosition,
         size = wx.Size(700, 480),
         style = wx.NO_BORDER | wx.SIMPLE_BORDER
      )

      self.ReadyToClose = True
      self.SetBackgroundColour(colors['background'])
      self.Centre(wx.BOTH)

      wx.StaticBitmap(self, bitmap=ScaledBitmap('./resources/background.png', 696, 451), pos = (0, 24), size = (164, 451))
      TitlePannel(self)
      self.contentPannel = InstallationStartPanel(self)

   def __del__( self ):
      pass

   def BeginInstall(self):
      self.ReadyToClose = False
      self.contentPannel.Destroy()
      self.contentPannel = InstallForm(self)
      self.contentPannel.BeginInstallation()

   def InstallSucess(self):
      self.ReadyToClose = True
      self.contentPannel.Destroy()
      self.contentPannel = InstallSucessPanel(self)


versionNumber = 'v1.0.1'
app = wx.App()

if os.path.exists('./fonts/RobotoSlab.ttf'):
   wx.Font.AddPrivateFont('./fonts/RobotoSlab.ttf')

window = MainWindow(None)
window.Show()

app.MainLoop()
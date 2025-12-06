using DW3ArduinoEditor.ViewModels;
using System.Windows;

namespace DW3ArduinoEditor
{
   public partial class MainWindow : Window
   {
      private readonly MainWindowViewModel _viewModel;

      public MainWindow()
      {
         InitializeComponent();
         _viewModel = new MainWindowViewModel();
         DataContext = _viewModel;
      }
   }
}
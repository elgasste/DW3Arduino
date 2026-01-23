using System.ComponentModel;
using System.Globalization;
using System.Windows.Data;

namespace DW3ArduinoEditor.Converters
{
   public class EnumDescriptionConverter : IValueConverter
   {
      public object? Convert( object value, Type targetType, object parameter, CultureInfo culture )
      {
         if ( value is Enum e )
         {
            var fieldInfo = e.GetType().GetField( e.ToString() );

            if ( fieldInfo is null )
            {
               return string.Empty;
            }

            var attributes = ( DescriptionAttribute[] )fieldInfo.GetCustomAttributes( typeof( DescriptionAttribute ), false );
            return ( attributes.Length > 0 ) ? attributes[0].Description : e.ToString();
         }

         return value?.ToString();
      }

      public object ConvertBack( object value, Type targetType, object parameter, CultureInfo culture )
      {
         throw new NotImplementedException();
      }
   }
}

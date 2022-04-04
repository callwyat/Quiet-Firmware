
import quiet_coms

class quite():
    
    def __init__(self, coms) -> None:
        
        if type(coms) == quiet_coms.QuietComs:
            self.coms = coms
        else:
            self.coms = quiet_coms.QuietComs(coms)

    def reset(self):
        self.coms.write('*RST')